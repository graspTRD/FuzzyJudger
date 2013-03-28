#include "stdafx.h"
#include "piccapture.h"
#include <QStyleOption>
#include <QPainter>
#include <QPixmap>

PicCapture::PicCapture(QWidget *parent)	: QWidget(parent)
{
	picLabel = NULL;
	picLabel = new QLabel(this);
	picLabel->setObjectName("picLabel");
	setObjectName("PicCapture");
	setStyleSheet("QWidget#PicCapture{background-color:black}");
}

PicCapture::~PicCapture()
{

}

void PicCapture::paintEvent( QPaintEvent* e)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	QWidget::paintEvent(e);
}

void PicCapture::resizeEvent( QResizeEvent* e)
{
	picLabel->setGeometry(0,0,width(),height());
}

void PicCapture::onshowPic( const QString& file )
{
	
	QFile f(file); 
	if(!f.exists()) return;
	//f.open();
	if(!f.open(QIODevice::ReadOnly)) return;
	QPixmap pixmap;
	pixmap.loadFromData(f.readAll());
	int w = pixmap.width();
	int h = pixmap.height();
	//picLabel->setStyleSheet("QLabel#picLabel{background-color:black}");
	if (w/(float)h >= width()/height())
	{
		picLabel->setFixedWidth(width());
		picLabel->setFixedHeight(width()*h/(float)w);
		
	}
	else
	{
		picLabel->setFixedWidth(w*height()/(float)h);
		picLabel->setFixedHeight(height());
	}
	picLabel->setGeometry((width() - picLabel->width())/2, (height() - picLabel->height())/2, picLabel->width(), picLabel->height());
	picLabel->setPixmap(pixmap.scaled(picLabel->width(), picLabel->height()));
}
