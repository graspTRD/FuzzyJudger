#include "stdafx.h"
#include "judgeworker.h"
#include "thumbnails.h"

#include <QStyleOption>
#include <QPainter>
#include <QLabel>
#include <QCheckBox>
#include <shellapi.h>
#include <QThread>


Thumbnails::Thumbnails(QWidget *parent)	: QWidget(parent)
{
	imgView = NULL;
	imgLayout = NULL;
	blurjudger = NULL;
	blurjudger = new BlurJudger();


	imgView = new QScrollArea(this);
	imgLayout = new QVBoxLayout(imgView);
	
	w = new QWidget(imgView);
	w->setLayout(imgLayout);
	imgLayout->setAlignment(Qt::AlignTop);
	imgView->setWidget(w);
 	imgView->setContentsMargins(0,0,0,0);
 	imgLayout->setSpacing(0);
 	imgLayout->setContentsMargins(0,0,0,0);
 	imgLayout->setAlignment(Qt::AlignLeft|Qt::AlignTop);
	imgView->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	imgView->setWidgetResizable(true);

	imgLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);

}

Thumbnails::~Thumbnails()
{
	if (blurjudger)
	{
		delete blurjudger;
		blurjudger = NULL;
	}
}


void Thumbnails::paintEvent(QPaintEvent* e)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	QWidget::paintEvent(e);
}

void Thumbnails::resizeEvent( QResizeEvent * )
{
	imgView->setGeometry(0,0,width(),height());
	imgView->setObjectName("aa");
	//imgView->setStyleSheet("QScrollArea#aa{background-color:red}");
}

void Thumbnails::oncreateThumbnails( const QFileInfoList& imgList )
{

	imglist.clear();
	QHBoxLayout* hlayout = NULL;
	QVector<QHBoxLayout*> hLayoutList;

	QFileInfoList  FileList;
	foreach(QFileInfo info, imgList)
	{
		bool flags = false;
		QString filename = 	QDir::toNativeSeparators(info.filePath());
		BEGIN_EXEC
		int m = blurjudger->Judge(filename, &flags);
		qDebug()<<"解析文件: "<<filename;
		END_EXEC
		if(m != 0) {
			qDebug()<<"解析文件出错"<<filename;
			continue;
		}
		if(flags == false) 
		{
			FileList.push_back(info);
		}
	}
	//FileList = imgList;
	int k = 0;
	k = FileList.length()%3==0 ? FileList.length()/3:FileList.length()/3 + 1;
	for(int i = 0; i < k; i++)
	{
		QHBoxLayout* hBox = new QHBoxLayout(w);
		hBox->setSpacing(3);
		hBox->setContentsMargins(3,0,0,0);
		//hBox->setAlignment(Qt::AlignLeft|Qt::AlignTop);
		imgLayout->insertLayout(i,hBox);
		//imgLayout->addStretch();
		hLayoutList.push_back(hBox);
	}
	int i = 0;
	foreach(QFileInfo info, FileList)
	{
		if (i%3 == 0)
		{
			hlayout = hLayoutList.at(i/3);
		}
		QVBoxLayout* vbox = new QVBoxLayout(w);
		vbox->setAlignment(Qt::AlignCenter);
		QCheckBox* checkbox = new QCheckBox(w);
		checkbox->setProperty("imgname", info.filePath());
		imglist.insert(0, checkbox);
		QLabel* lab = new QLabel(w);
		lab->setFixedWidth(imgView->width()/3 - 15);
		lab->setFixedHeight(imgView->width()/3 - 15);
		vbox->addWidget(lab);
		lab->setProperty("img", info.filePath());
		lab->installEventFilter(this);

		QPixmap pp;
		pp.loadFromData(FileUtils::getThumbnail(info.filePath()));
		lab->setPixmap(pp.scaled(imgView->width()/3 - 15,imgView->width()/3 - 15));
		vbox->addWidget(checkbox);

		hlayout->addLayout(vbox);

		if(i%3 == 2) imgLayout->addLayout(hlayout);

		i++;
	}
}

bool Thumbnails::eventFilter( QObject * o, QEvent * e )
{
	if(e->type() == QEvent::MouseButtonPress)
	{
		if (o->property("img").toString() != "")
		{
			emit showPic(o->property("img").toString());
		}
	}
	return false;
}

void Thumbnails::ondealPic()
{
	foreach(QCheckBox* box, imglist)
	{
		QString ss = box->property("imgname").toString();
		ss = ss.replace("/", "\\");
		//ss += "\r\n";

		SHFILEOPSTRUCT FileOp={0}; 
		FileOp.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION; //放入回收站&&不出现确认对话框
		std::wstring rt = ss.toStdWString();
		FileOp.pFrom = rt.c_str();
		//FileOp.pFrom = L"G:\\Users\\shenghai\\Desktop\\bbbcc\\ 37.jpg\0";
		FileOp.pTo = NULL;      //一定要是NULL
		FileOp.wFunc = FO_DELETE;    //删除操作
 		//这里开始删除文件
		bool nOk = ::SHFileOperation(&FileOp);
		DWORD d = ::GetLastError();
		if(nOk)
			qDebug()<<"删除失败";
		else
			qDebug()<<"删除成功";
	}

}

void Thumbnails::onJudgePictures( const QDir& dir, int force )
{
	QThread* thread = new QThread(this);
	JudgeWorker* worker = new JudgeWorker(dir, force);

	worker->moveToThread(thread);
	worker->connect(thread, SIGNAL(started()), SLOT(doJudge()));
	worker->connect(worker, SIGNAL(finished()), SLOT(deleteLater()));
	thread->connect(worker, SIGNAL(finished()), SLOT(quit()));
	thread->connect(thread, SIGNAL(finished()), SLOT(deleteLater()));

	this->connect(worker, SIGNAL(itemFinished(const QString&, bool)), SLOT(slotItemFinished(const QString&, bool)), Qt::QueuedConnection);
	this->connect(worker, SIGNAL(itemError(const QString&, int)), SLOT(slotItemError(const QString&, int)), Qt::QueuedConnection);

	thread->start();

}

void Thumbnails::slotItemError( const QString& picName, int code )
{
	qDebug()<<"文件："<<picName<<"判断错误！"; //;
	qDebug()<<"Code： "<<code;  //;
}

void Thumbnails::slotItemFinished( const QString& picName, bool ret )
{
	//TODO 追加到视图;
}


