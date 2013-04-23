#include "stdafx.h"
#include "syspanel.h"
#include "pichandle.h"
SysPanel::SysPanel(QWidget *parent) : QWidget(parent)
{
	m_syspanel = NULL;
	m_closeBtn = NULL;
	m_maxBtn = NULL;
	m_minBtn = NULL;

	PicHandle* pichandle = dynamic_cast<PicHandle*>(parent);
	if(!pichandle) return;
	QRect rec = pichandle->rect();
	setAttribute(Qt::WA_TranslucentBackground);
	setGeometry(QRect(rec.right() - 119 ,rec.top() + 1,119,17));
	setFocusPolicy(Qt::NoFocus);

	m_syspanel = new QHBoxLayout(this);
	m_syspanel->setContentsMargins(0, 0, 0, 0);


	if(!m_minBtn) m_minBtn = new QPushButton(this);
	m_syspanel->addWidget(m_minBtn,1);
	m_minBtn->setFixedHeight(17);
	m_minBtn->setFixedWidth(25);
	m_minBtn->setProperty("minBtn",QVariant(true));
	m_minBtn->setAttribute(Qt::WA_TranslucentBackground);
	connect(m_minBtn,SIGNAL(clicked()),this,SLOT(OnSysMin()));

	if(!m_maxBtn) m_maxBtn = new QPushButton(this);
	m_syspanel->addWidget(m_maxBtn,2);
	m_maxBtn->setFixedHeight(17);
	m_maxBtn->setFixedWidth(26);
	m_maxBtn->setProperty("normalBtn",QVariant(true));
	m_maxBtn->setAttribute(Qt::WA_TranslucentBackground);
	connect(m_maxBtn,SIGNAL(clicked()),this,SLOT(OnSysMaxMin()));

	if(!m_closeBtn)m_closeBtn = new QPushButton(this);
	m_syspanel->addWidget(m_closeBtn,3);
	m_closeBtn->setFixedHeight(17);
	m_closeBtn->setFixedWidth(42);
	m_closeBtn->setProperty("closeBtn",QVariant(true));
	m_closeBtn->setAttribute(Qt::WA_TranslucentBackground);
	connect(m_closeBtn,SIGNAL(clicked()),this,SLOT(OnSysClose()));

	//m_numLayout->setSpacing(0);
	m_syspanel->setAlignment(Qt::AlignTop|Qt::AlignRight);
	m_syspanel->setSpacing(0);
	m_syspanel->setGeometry(QRect(rect().right() ,rect().top() + 20,119,17));
	
	connect(parent,SIGNAL(sysReducted()),this,SLOT(OnSysReducted()));
	connect(parent,SIGNAL(sysMaxed()),this,SLOT(OnSysMax()));

	show();
}

SysPanel::~SysPanel()
{

}

void SysPanel::mousePressEvent(QMouseEvent* e)
{
	
	e->accept();
}

void SysPanel::mouseMoveEvent(QMouseEvent* e)
{
	QWidget::mouseMoveEvent(e);
}

void SysPanel::paintEvent(QPaintEvent* e)
{
}

void SysPanel::OnSysClose()
{
	emit sysClose();
}

void SysPanel::OnSysMin()
{
	emit sysMin();
}

void SysPanel::OnSysMaxMin()
{
	
	QPushButton *pBtn = dynamic_cast<QPushButton *>(sender());
	if(!pBtn) return;
	QVariant var = pBtn->property("normalBtn");
	if(var.toBool()) pBtn->setProperty("normalBtn",false);
	else pBtn->setProperty("normalBtn",true);

	QFile file(":/sysmaxbutton.qss");
	file.open(QFile::ReadOnly);
	QString style = QString(file.readAll());
	pBtn->setStyleSheet(style);
	file.close();
	emit sysMaxMin();

}

void SysPanel::OnSysReducted()
{
	m_maxBtn->setProperty("normalBtn",true);

	QFile file(":/sysmaxbutton.qss");
	file.open(QFile::ReadOnly);
	QString style = QString(file.readAll());
	m_maxBtn->setStyleSheet(style);
}

void SysPanel::OnSysMax()
{
	m_maxBtn->setProperty("normalBtn",false);

	QFile file(":/sysmaxbutton.qss");
	file.open(QFile::ReadOnly);
	QString style = QString(file.readAll());
	m_maxBtn->setStyleSheet(style);
}
