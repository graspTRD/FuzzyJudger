#include "stdafx.h"
#include "thumbnails.h"
#include "judgeworker.h"
#include <QStyleOption>
#include <QPainter>
#include <QLabel>
#include <QCheckBox>
#include <windows.h>
#include <shellapi.h>
#include<stdio.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <ctype.h>
#include <QThread>

Thumbnails::Thumbnails(QWidget *parent)	: QWidget(parent)
{
	picNum = 0;
	imgView = NULL;
	imgLayout = NULL;
	blurjudger = NULL;
	hlayout = NULL;
	blurjudger = new BlurJudger();

	imgView = new QScrollArea(this);
	imgView->setObjectName("imgView");
	QScrollBar* vBar = imgView->verticalScrollBar();//new QScrollBar(scroll);
	vBar->setObjectName("VScroll");
	vBar->setFixedWidth(4);


	QScrollBar* hBar = imgView->horizontalScrollBar();
	hBar->hide();

	w = new QWidget(imgView);
	w->setObjectName("w");
	//imgView->setStyleSheet("QScrollArea#imgView{background-color:red}");
	w->setStyleSheet("QWidget#w{background-color:gray}");
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
	
	w->setGeometry(imgView->rect());
}


void Thumbnails::oncreateThumbnails( const QDir& dir )
{
	
	picNum = 0;
	imglist.clear();

	imgLayout = new QVBoxLayout(imgView);	
	imgLayout->setSpacing(3);
	imgLayout->setContentsMargins(3,3,3,3);
	imgLayout->setAlignment(Qt::AlignLeft|Qt::AlignTop);
	imgLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);

	imgView->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	imgView->setWidgetResizable(true);
	
	imgView->setWidget(w);
	w->setLayout(imgLayout);

	onJudgePictures(dir, 0);
	/*
	QHBoxLayout* hlayout = NULL;
	QVector<QHBoxLayout*> hLayoutList;

	QFileInfoList  FileList;

	FileList = imgList;
	int k = 0;
	k = FileList.length()%3==0 ? FileList.length()/3:FileList.length()/3 + 1;
	for(int i = 0; i < k; i++)
	{
		QHBoxLayout* hBox = new QHBoxLayout(w);
		hBox->setSpacing(5);
		hBox->setContentsMargins(3,3,3,3);
		hBox->setAlignment(Qt::AlignLeft|Qt::AlignTop);
		imgLayout->addLayout(hBox);
		imgLayout->addStretch();
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
		vbox->setAlignment(Qt::AlignHCenter);
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
		i++;
	}*/
}

void Thumbnails::slotItemFinished( const QString& picName, bool ret )
{
	//TODO 追加到视图;
	//static 
	qDebug()<<picName<< ret;
	if(ret) return;
	if(picNum % 3 == 0)
	{
		QHBoxLayout* layout = new QHBoxLayout(w);
		hlayout = layout;
	}

	QVBoxLayout* vbox = new QVBoxLayout(w);
	vbox->setAlignment(Qt::AlignHCenter);
	QCheckBox* checkbox = new QCheckBox(w);
	checkbox->setProperty("imgname", picName);
	imglist.insert(0, checkbox);
	QLabel* lab = new QLabel(w);
	lab->setFixedWidth(imgView->width()/3 - 15);
	lab->setFixedHeight(imgView->width()/3 - 15);
	vbox->addWidget(lab);
	lab->setProperty("img", picName);
	lab->installEventFilter(this);

	QPixmap pp;
	pp.loadFromData(FileUtils::getThumbnail(picName));
	lab->setPixmap(pp.scaled(imgView->width()/3 - 15,imgView->width()/3 - 15));
	vbox->addWidget(checkbox);

	hlayout->addLayout(vbox);
	if(picNum % 3 == 2 ) imgLayout->addLayout(hlayout);
	picNum++;
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
		//QFile::remove(ss);
		ss = QDir::toNativeSeparators(ss);
		//ss = ss.replace("\\", "\\\\");
		//ss.clear();
		//ss = "G:\\Users\\shenghai\\Desktop\\201301\\IMG_20130109_09452400.jpg";
		
		SHFILEOPSTRUCTW FileOp;
		ZeroMemory(&FileOp, sizeof(SHFILEOPSTRUCTW));
		FileOp.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION; //放入回收站&&不出现确认对话框

#ifdef UNICODE
		std::wstring stemp = ss.toStdWString(); // Temporary buffer is required
		LPCWSTR result = stemp.c_str();
#else
		LPCWSTR result = s.c_str();
#endif

		//std::wstring rt = s2ws(ss.toStdString());


		//LPCWSTR lpwStr= (LPCWSTR)rt.c_str();
		//ss = "L\"" + ss + "\"";
		FileOp.pFrom = result;
		//FileOp.pFrom = _T("G:\\Users\\shenghai\\Desktop\\201301\\IMG_20130109_09452400.jpg");
		FileOp.pTo = NULL;      //一定要是NULL
		FileOp.wFunc = FO_DELETE;    //删除操作
 		//这里开始删除文件
 		int nOk = SHFileOperationW(&FileOp); // 返回2 系统找不到指定文件
		if(nOk)
			qDebug()<<"删除失败";
		else
			qDebug()<<"删除成功";
	}

}

// std::wstring Thumbnails::s2ws(const std::string& s)
// {
// 	int len;
// 	int slength = (int)s.length() + 1;
// 	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
// 	wchar_t* buf = new wchar_t[len];
// 	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
// 	std::wstring r(buf);
// 	delete[] buf;
// 	return r;
// }

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




