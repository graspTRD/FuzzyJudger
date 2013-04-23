#include "stdafx.h"
#include "piccapture.h"
#include <QStyleOption>
#include <QPainter>
#include <QPixmap>

PicCapture::PicCapture(QWidget *parent)	: QWidget(parent)
{
	filename = "";
	picLabel = NULL;
	picdetail = NULL;

	view = NULL;
	scene = NULL;
	pixmapItem = NULL;

	view = new GraphicsView(this);
	view->setDragMode(QGraphicsView::ScrollHandDrag);
	view->setTransformationAnchor(QGraphicsView::NoAnchor);
	view->setAlignment(Qt::AlignCenter);
	view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	scene = new QGraphicsScene;
	pixmapItem = new QGraphicsPixmapItem;
	pixmapItem->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
	pixmapItem->setTransformationMode(Qt::SmoothTransformation);

	scene->addItem(pixmapItem);
	view->setScene(scene);
	

// 	picLabel = new QLabel(this);
// 	picLabel->setObjectName("picLabel");
// 	setObjectName("PicCapture");
//	setStyleSheet("QWidget#PicCapture{background-color:black}");
	view->setObjectName("view");
	view->setStyleSheet("QWidget#view{background-color:black}");
	connect(view, SIGNAL(dbclicked()), SLOT(ondbclicked()));
	//view->installEventFilter(this);
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
// 	picLabel->setGeometry(rect());
// 	if(filename != "") onshowPic(filename);
	view->setGeometry(rect());
	if(filename != "") onshowPic(filename);
}

void PicCapture::onshowPic( const QString& file )
{
	

	
	QFile f(file); 
	if(!f.exists()) return;
	//f.open();
	if(!f.open(QIODevice::ReadOnly)) return;
	filename = file;
	QPixmap pixmap;
	pixmap.loadFromData(f.readAll());
	pixmapItem->setPixmap(pixmap);
	QSize photoSize = pixmap.size();
	view->setSceneRect(0, 0, photoSize.width(), photoSize.height());
	view->setAlignment(Qt::AlignCenter);
// 	if (photoSize.width() < view->width() && photoSize.height() < view->height())
// 	{
// 		photoRotate = 0;
// 		zoomValue = 250;
// 		setupMatrix();
// 		return;
// 	}
	view->fitInView(pixmapItem, Qt::KeepAspectRatio);
	/*
	QFile f(file); 
	if(!f.exists()) return;
	//f.open();
	if(!f.open(QIODevice::ReadOnly)) return;
	filename = file;
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
	picLabel->setPixmap(pixmap.scaled(picLabel->width(), picLabel->height()));*/
}

bool PicCapture::eventFilter( QObject* o, QEvent* e )
{
	return false;
}

void PicCapture::onExc()
{
	if (picdetail)
	{
		delete picdetail;
		picdetail = NULL;
	}
}

void PicCapture::ondbclicked()
{
	picdetail = new PicDetail();
	picdetail->showPic(filename);
	connect(picdetail, SIGNAL(exc()), SLOT(onExc()));
}

//////////////////////////////////////////////////////////////////////////
GraphicsView::GraphicsView(QWidget* parent) : QGraphicsView(parent)
{

}

GraphicsView::~GraphicsView()
{

}

void GraphicsView::mouseDoubleClickEvent( QMouseEvent *event )
{
	emit dbclicked();
}


