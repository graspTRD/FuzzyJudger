#include "stdafx.h"
#include "pichandle.h"
#include "jhead/jpghead.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QStyleOption>

PicHandle::PicHandle(QWidget *parent) : QWidget(parent)
{
	this->setObjectName("PicHandle");
	this->setWindowTitle("文件改名器");

	//this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
	setAttribute(Qt::WA_TranslucentBackground);
	
	
	this->setFixedWidth(1024);
	this->setFixedHeight(768);
	
	initUI();
}

PicHandle::~PicHandle()
{

}

void PicHandle::initUI()
{
	controllbar = NULL;
	controllbar = new ControllBar(this);
	thumbnails = NULL;
	thumbnails = new Thumbnails(this);
	piccapture = NULL;
	piccapture = new PicCapture(this);

	controllbar->setGeometry(10,10, 450, 150);
	connect(controllbar, SIGNAL(createThumbnails(const QDir&, int)), thumbnails, SLOT(oncreateThumbnails(const QDir&, int)));

	thumbnails->setGeometry(470, 10, width() - 480, height() - 20);
	piccapture->setGeometry(10, 160, 450, height() - 170);
	connect(thumbnails, SIGNAL(showPic(const QString&)), piccapture, SLOT(onshowPic(const QString&)));
 	connect(controllbar, SIGNAL(dealPic()), thumbnails, SLOT(ondealPic()));
 	connect(thumbnails, SIGNAL(piccount(int)), controllbar, SLOT(onsetpiccount(int)));
 	connect(thumbnails, SIGNAL(picstepchanged(int)), controllbar, SLOT(onpicstepchanged(int)));
	connect(thumbnails, SIGNAL(picdealfinished()), controllbar, SLOT(onpicdealfinished()));
}

void PicHandle::paintEvent(QPaintEvent* e)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	QWidget::paintEvent(e);
}
