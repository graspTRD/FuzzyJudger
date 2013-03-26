#include "stdafx.h"
#include "piccapture.h"
#include <QStyleOption>
#include <QPainter>
#include <QPixmap>

PicCapture::PicCapture(QWidget *parent)	: QWidget(parent)
{
	picLabel = NULL;
	picLabel = new QLabel(this);
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
	QPixmap pixmap(file);
	picLabel->setPixmap(pixmap.scaled(picLabel->width(), picLabel->height()));
}
