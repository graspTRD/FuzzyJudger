#include "stdafx.h"
#include "viewer.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtDeclarative/QDeclarativeComponent>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeContext>
#include <QPainter>
#include <QDebug>
//#include <qplatformdefs.h> // MEEGO_EDITION_HARMATTAN

#include "../common/widgetrect.h"

// class ViewerPrivate
// {
//     QString mainQmlFile;
//     friend class Viewer;
//     static QString adjustPath(const QString &path);
// };
// 
// QString ViewerPrivate::adjustPath(const QString &path)
// {
//     return path;
// }

Viewer::Viewer(QWidget *parent) : QWidget(parent)
{
//     connect(engine(), SIGNAL(quit()), SLOT(close()));
//     setResizeMode(QDeclarativeView::SizeRootObjectToView);
    setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);  // 窗体符合背景圆角

	

    setMouseTracking(true);
	//setStyleSheet("Viewer{border-image: url(:/mainWnd.png)}");
	//./Resources/image
    //setWindowOpacity(0.5);
	setObjectName("tt");
	setStyleSheet("QWidget#tt{border-image: url(\":/image/selpic/tablewidget.png\");}");
    m_bMaxed = false;
	isMoveable = false;
}

Viewer::~Viewer()
{
    //delete m_d;
}

// void Viewer::setMainQmlFile(const QString &file)
// {
//     //setSource(QUrl(file));
// }

// void Viewer::addImportPath(const QString &path)
// {
//     engine()->addImportPath(ViewerPrivate::adjustPath(path));
// }

void Viewer::setOrientation(ScreenOrientation orientation)
{
    Qt::WidgetAttribute attribute;
    switch (orientation) {

    case ScreenOrientationLockPortrait:
        attribute = Qt::WA_LockPortraitOrientation;
        break;
    case ScreenOrientationLockLandscape:
        attribute = Qt::WA_LockLandscapeOrientation;
        break;
    default:
    case ScreenOrientationAuto:
        attribute = Qt::WA_AutoOrientation;
        break;
    };
    setAttribute(attribute, true);
}

void Viewer::showExpanded()
{
    show();
}

void Viewer::mousePressEvent(QMouseEvent* e)
{

	isMoveable = isclickeAvalibleArea(e->globalPos());
	m_recordPt = e->globalPos();
	if (e->button() == Qt::LeftButton) m_ptPressGlobal = e->globalPos();
	//e->ignore();
	QWidget::mousePressEvent(e);
}

void Viewer::mouseReleaseEvent(QMouseEvent* e)
{
	isMoveable = false;
    if (e->button() == Qt::LeftButton) m_eDirection = eNone;
	QWidget::mouseReleaseEvent(e);
}

void Viewer::mouseMoveEvent(QMouseEvent* e)
{
    if(!e->buttons()&Qt::LeftButton)// && !m_bMaxed
    {
        m_eDirection = PointValid(e->x(),e->y());
        SetCursorStyle(m_eDirection);
    }
    if(e->buttons()&Qt::LeftButton)// && !m_bMaxed
    {
        int nXGlobal = e->globalX();
        int nYGlobal = e->globalY();
        SetDrayMove(nXGlobal,nYGlobal,m_eDirection, QPoint(e->x(),e->y()));
        m_ptPressGlobal =QPoint(nXGlobal,nYGlobal);
    }
    //m_isPressinMain &&
    if (e->buttons()&Qt::LeftButton  /*&& isclickeAvalibleArea(e->globalPos())*/ && isMoveable && m_eDirection == eNone )
    {
        if(m_bMaxed)
        {
            //emit sysReducted();
            //setGeometry(e->globalPos().x() - m_normalRect.width()/2,e->globalPos().y() - 15,m_normalRect.width(),m_normalRect.height());
            m_bMaxed = false;
            this->repaint();
        }
        else
        {
            this->move(mapToGlobal(rect().topLeft()) + e->globalPos() - m_recordPt);
            m_recordPt = e->globalPos();
        }
    }
	//e->ignore();
	QWidget::mouseMoveEvent(e);
}

void Viewer::SetCursorStyle(enum_Direction direction)
{
	//设置上下左右以及右上、右下、左上、坐下的鼠标形状
	switch(direction)
	{
	case eTop:
	case eBottom:
		setCursor(Qt::SizeVerCursor);
		break;
	case eRight:
	case eLeft:
		setCursor(Qt::SizeHorCursor);
		break;
	case eTopRight:
	case eBottomLeft:
		setCursor(Qt::SizeBDiagCursor);
		break;
	case eRightBottom:
	case eLeftTop:
		setCursor(Qt::SizeFDiagCursor);
		break;
	default:
		setCursor(Qt::ArrowCursor);
		break;
	}
}

void Viewer::SetDrayMove(int nXGlobal,int nYGlobal,enum_Direction direction,QPoint pos)  //  拖动边框  放大缩小窗口
{
	//计算偏差
	int ndX = nXGlobal - m_ptPressGlobal.x();
	int ndY = nYGlobal - m_ptPressGlobal.y();

	enum_Direction dire = PointValid(pos.x(),pos.y());
	//获得主窗口位置信息
	QRect rectWindow = geometry();
	//判别方向
	if(direction == eTop)
	{
		if(ndY < 0 && dire == eNone ) return;
		rectWindow.setTop(rectWindow.top()+ndY);
	}
	else if(direction == eRight)
	{
		if(ndX > 0 && dire == eNone ) return;
		rectWindow.setRight(rectWindow.right()+ndX);
	}
	else if(direction == eBottom)
	{
		if(ndY > 0 && dire == eNone ) return;
		rectWindow.setBottom(rectWindow.bottom()+ndY);
	}
	else if(direction == eLeft)
	{
		if(ndX < 0 && dire == eNone ) return;
		rectWindow.setLeft(rectWindow.left()+ndX);
	}
	else if (direction == eLeftTop)
	{
		if( (ndX <= 0 || ndY <= 0) && dire == eNone ) return;

		rectWindow.setLeft(rectWindow.left()+ndX);
		rectWindow.setTop(rectWindow.top()+ndY);
	}
	else if (direction == eTopRight)
	{
		if( (ndX >= 0 || ndY <= 0) && dire == eNone ) return;
		rectWindow.setRight(rectWindow.right()+ndX);
		rectWindow.setTop(rectWindow.top()+ndY);
	}
	else if (direction == eBottomLeft)
	{
		if( (ndX <= 0 || ndY >= 0) && dire == eNone ) return;
		rectWindow.setLeft(rectWindow.left()+ndX);
		rectWindow.setBottom(rectWindow.bottom()+ndY);
	}
	else if (direction == eRightBottom)
	{
		if( (ndX >= 0 || ndY >= 0) && dire == eNone ) return;
		rectWindow.setRight(rectWindow.right()+ndX);
		rectWindow.setBottom(rectWindow.bottom()+ndY);
	}
	if(rectWindow.width()< minimumWidth() || rectWindow.height()<minimumHeight())
	{
		return;
	}
	//重新设置窗口位置为新位置信息
	setGeometry(rectWindow);
}

Viewer::enum_Direction Viewer::PointValid(int x,int y)
{
	if (x < -30 || y < -20 || x > width() + 30 || y > height() + 20)
	{
		return eNone;
	}
	else if ( x - borderWidth < 0 && y - borderWidth >0 && y + borderWidth < height() )  // left
	{
		return eLeft;
	}
	else if ( x + borderWidth > width() && y - borderWidth >0 && y + borderWidth < height() &&  y > 17 + 2 && height() - y > 15 + borderWidth)
	{
		return eRight;
	}
	else if ( y - borderWidth < 0 && x - borderWidth >0 && x + borderWidth < width() && width() - x > 40 + 2)
	{
		return eTop;
	}
	else if ( y + borderWidth > height() && x - borderWidth >0 && x + borderWidth < width() && width() - x > 15 + borderWidth )
	{
		return eBottom;
	}
	else if (x - borderWidth < 0 && y - borderWidth <0 )
	{
		return eLeftTop;//;
	}
	else if (x + borderWidth + 8 > width() && y + borderWidth + 8 > height() )
	{
		return eRightBottom;
	}
	else if (x - borderWidth < 0 && y + borderWidth > height() )
	{
		return eBottomLeft;
	}
	else if (x + borderWidth > width() && y - borderWidth < 0 )
	{
		return eTopRight;//;
	}
	else return eNone;
}

bool Viewer::isclickeAvalibleArea(const QPoint& pt)
{
    QRect rect = WidgetRect::widgetGlobalRect(this);
    rect.setX(rect.x() - borderWidth*2);
    rect.setY(rect.y() - borderWidth*2);
    rect.setWidth(rect.width() - borderWidth*4);
    rect.setHeight(50 - borderWidth*2);
	(rect.contains(pt))? isMoveable = true:isMoveable = false;
    return (rect.contains(pt))? true:false;
}

void Viewer::paintEvent(QPaintEvent* e)
{
	/*QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	e->ignore();*/
	QWidget::paintEvent(e);
}

bool Viewer::isDbclickHead(const QPoint& pt)
{
	QRect rect = WidgetRect::widgetGlobalRect(this);
	rect.setHeight(25);
	return (rect.contains(pt))? true:false;
}

