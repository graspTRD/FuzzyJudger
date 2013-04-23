#include "stdafx.h"
#include "pichandle.h"
#include "jhead/jpghead.h"
#include "common/widgetrect.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QStyleOption>
#include <QDesktopWidget>
#include <QRect>

PicHandle::PicHandle(QWidget *parent) : Viewer(parent)
{
	this->setObjectName("PicHandle");
	this->setWindowTitle("文件改名器");

	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
	setAttribute(Qt::WA_TranslucentBackground);
	
	m_bMaxed = true;
	setMinimumWidth(940);
	setMinimumHeight(720);
	setMaximumSize(QApplication::desktop()->availableGeometry().size());
	initUI();
}

PicHandle::~PicHandle()
{

}

void PicHandle::initUI()
{
 	QRect rec = QApplication::desktop()->availableGeometry();

	QRect r(rec.width()/2 - 1024/2, rec.height()/2 - 768/2, 1024, 768); // 放弃分辨率小于1024*768
	m_normalRect = r;
	syspanel = NULL;
	controllbar = NULL;
	controllbar = new ControllBar(this);
	thumbnails = NULL;
	thumbnails = new Thumbnails(this);
	piccapture = NULL;
	piccapture = new PicCapture(this);

	syspanel = new SysPanel(this);
	connect(syspanel,SIGNAL(sysClose()),this,SLOT(OnSysClose()));
	connect(syspanel,SIGNAL(sysMin()),this,SLOT(OnSysMin()));
	connect(syspanel,SIGNAL(sysMaxMin()),this,SLOT(OnSysMaxMin()));

	controllbar->setGeometry(10,10, 450, 150);
	connect(controllbar, SIGNAL(createThumbnails(const QDir&, int)), thumbnails, SLOT(oncreateThumbnails(const QDir&, int)));

	thumbnails->setGeometry(470, 10, width() - 480, height() - 20);
	piccapture->setGeometry(10, 160, 450, height() - 170);
	connect(thumbnails, SIGNAL(showPic(const QString&)), piccapture, SLOT(onshowPic(const QString&)));
 	connect(controllbar, SIGNAL(dealPic()), thumbnails, SLOT(ondealPic()));
 	connect(thumbnails, SIGNAL(piccount(int)), controllbar, SLOT(onsetpiccount(int)));
 	connect(thumbnails, SIGNAL(picstepchanged(int)), controllbar, SLOT(onpicstepchanged(int)));
	connect(thumbnails, SIGNAL(picdealfinished()), controllbar, SLOT(onpicdealfinished()));
	//this->showMaximized();
	this->setGeometry(QApplication::desktop()->availableGeometry());
}

void PicHandle::paintEvent(QPaintEvent* e)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	QWidget::paintEvent(e);
}

void PicHandle::resizeEvent( QResizeEvent* e)
{
	if (!syspanel) return;
	syspanel->setGeometry(QRect(rect().right() - 119 ,rect().top() + 1,119,17));
	if (!controllbar) return;
	controllbar->setGeometry(10,25, 450, 150);
	if (!thumbnails) return;
	thumbnails->setGeometry(470, 25, width() - 480 , height() - 35);
	if (!piccapture) return;
	piccapture->setGeometry(10, 160, 450, height() - 170);
}

void PicHandle::mouseDoubleClickEvent(QMouseEvent* e)
{
	//bool k = getv(e->globalPos());
	//isDbclickHead(e->globalPos());
	if(!m_bMaxed && Viewer::isDbclickHead(e->globalPos()))
	{

		// 		m_normalRect = WidgetRect::widgetGlobalRect(this);
		// 		setGeometry(QApplication::desktop()->availableGeometry());

		//m_bMaxed = true;
		OnSysMaxMin();
		repaint();
	}
	else if(m_bMaxed && Viewer::isDbclickHead(e->globalPos()))
	{

		//setGeometry(m_normalRect);
		//m_bMaxed = false;
		OnSysMaxMin();
		repaint();
	}
}

void PicHandle::OnSysMin()
{
	showMinimized();
	this->setFocus();
	repaint();
}

void PicHandle::OnSysMaxMin()
{
	// 	SysPanel *pSysPanel = m_syspanel;//dynamic_cast<SysPanel *>(sender());
	// 	if(!pSysPanel) return;
	// 	bool ismax = pSysPanel->getSysState().toBool();
	if(/*ismax && */m_bMaxed) // 正常状态
	{
		setGeometry(m_normalRect);
		m_bMaxed = false;
	}
	else  // 最大化
	{
		m_normalRect = WidgetRect::widgetGlobalRect(this);
		setGeometry(QApplication::desktop()->availableGeometry());
		m_bMaxed = true;
	}
	this->setFocus();
	repaint();
}

void PicHandle::OnSysClose()
{
	close();
}


