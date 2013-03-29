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
	testnum = 0;
	imgView = NULL;
	blurjudger = NULL;
	picsLayout = NULL;
	w = NULL;
	blurjudger = new BlurJudger();

	imgView = new QScrollArea(this);
	imgView->setObjectName("imgView");
	imgView->setStyleSheet("QScrollArea#imgView{background-color:gray}");
	QScrollBar* vBar = imgView->verticalScrollBar();//new QScrollBar(scroll);
	vBar->setObjectName("VScroll");
	vBar->setFixedWidth(4);


	QScrollBar* hBar = imgView->horizontalScrollBar();
	hBar->hide();

	
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
	
	
}


void Thumbnails::oncreateThumbnails( const QDir& dir , int value)
{
	
	picNum = 0;
	testnum = 0;
	imglist.clear();

	QStringList filters;
	filters<<"*.BMP"<<"*.JPG"<<"*.JPEG"<<"*.PNG"<<"*.GIF";
	const_cast<QDir&>(dir).setNameFilters(filters);
	QFileInfoList picList = dir.entryInfoList();
	emit piccount(picList.length());

	if( picsLayout )
	{
		delete picsLayout;
		picsLayout = NULL;
	}
	if (w)
	{
		delete w;
		w = NULL;
	}
	w = new QWidget(imgView);
	w->setObjectName("w");
	
	w->setStyleSheet("QWidget#w{background-color:gray}");
	w->setGeometry(imgView->rect());

	picsLayout = new QGridLayout(imgView);
	picsLayout->setSpacing(8);
	picsLayout->setContentsMargins(5,5,5,5);
	picsLayout->setAlignment(Qt::AlignLeft|Qt::AlignTop);
	picsLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);

	imgView->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	imgView->setWidgetResizable(true);
	
	imgView->setWidget(w);
	w->setLayout(picsLayout);

	onJudgePictures(dir, value);
}

void Thumbnails::slotItemFinished( const QString& picName, bool ret )
{
	//TODO 追加到视图;
	 
	emit picstepchanged(testnum + 1);
	testnum ++;
	if(ret) return;

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

	picsLayout->addLayout(vbox, picNum/3, picNum % 3);
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
	QString str("G:/Users/shenghai/Desktop/bbbcc/ 1.jpg");
	LPCWSTR  lstr = (LPCWSTR)str.utf16();


// 	foreach(QCheckBox* box, imglist)
// 	{
// 		QString ss = box->property("imgname").toString();
// 		ss = QDir::toNativeSeparators(ss);

		SHFILEOPSTRUCT FileOp;
		ZeroMemory(&FileOp, sizeof(SHFILEOPSTRUCT));
		FileOp.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION; //放入回收站&&不出现确认对话框
		//LPCTSTR tt = ss.toLocal8Bit().data_ptr();
// 		char* rr = new char(100);
// 		memset(rr,100, 0);
// 		rr = ss.toLatin1().data();
		FileOp.pFrom = lstr;
		FileOp.pTo = NULL;      //一定要是NULL
		FileOp.wFunc = FO_DELETE;    //删除操作
 		//这里开始删除文件
 		int nOk = SHFileOperation(&FileOp); // 返回2 系统找不到指定文件
		if(nOk)
			qDebug()<<"删除失败";
		else
			qDebug()<<"删除成功";
	//}
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
	this->connect(worker, SIGNAL(finished()), SIGNAL(picdealfinished()), Qt::QueuedConnection);

	thread->start();
	thread->start();

}

void Thumbnails::slotItemError( const QString& picName, int code )
{
// 	qDebug()<<"文件："<<picName<<"判断错误！"; //;
// 	qDebug()<<"Code： "<<code;  //;
}




