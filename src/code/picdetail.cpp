#include "stdafx.h"
#include "picdetail.h"
#include <QShortcut>
#include <QKeySequence>

PicDetail::PicDetail(QWidget *parent) : QWidget(parent)
{
	pic = NULL;
	pic = new QLabel(this);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);

	this->setGeometry(QApplication::desktop()->availableGeometry());
	QShortcut *shortcut = new QShortcut(QKeySequence(Qt::Key_Escape),this);
	connect(shortcut, SIGNAL(activated()), this, SIGNAL(exc()));
	setObjectName("PicDetail");
	setStyleSheet("QWidget#PicDetail{background-color:black}");
}

PicDetail::~PicDetail()
{
	
}

void PicDetail::showPic( const QString& img )
{
	

	//this->showMaximized();
	if(!pic) 
	{
		pic = new QLabel(this);
	}

	QFile f(img); 
	if(!f.exists()) return;
	//f.open();
	if(!f.open(QIODevice::ReadOnly)) return;
	//filename = file;
	QPixmap pixmap;
	pixmap.loadFromData(f.readAll());
	int w = pixmap.width();
	int h = pixmap.height();
	//picLabel->setStyleSheet("QLabel#picLabel{background-color:black}");
	if (w >= width() && h >= height())
	{
		if (w/(float)h >= width()/height())
		{
			pic->setFixedWidth(width());
			pic->setFixedHeight(width()*h/(float)w);

		}
		else
		{
			pic->setFixedWidth(w*height()/(float)h);
			pic->setFixedHeight(height());
		}
	}
	else
	{
// 		if (w/(float)h >= width()/height())
// 		{
// 			pic->setFixedWidth(w);
// 			pic->setFixedHeight(w*height()/(float)width());
// 
// 		}
// 		else
// 		{
// 			pic->setFixedWidth(width()*h/(float)height());
// 			pic->setFixedHeight(h);
// 		}
		pic->setFixedWidth(w);
		pic->setFixedHeight(h);
	}
	
	pic->setGeometry((width() - pic->width())/2, (height() - pic->height())/2, pic->width(), pic->height());
	pic->setPixmap(pixmap.scaled(pic->width(), pic->height()));
	show();
	
}
