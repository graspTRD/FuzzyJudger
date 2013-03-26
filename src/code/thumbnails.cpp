#include "stdafx.h"
#include "thumbnails.h"
#include <QStyleOption>
#include <QPainter>
#include <QLabel>

Thumbnails::Thumbnails(QWidget *parent)	: QWidget(parent)
{
	imgView = NULL;
	imgLayout = NULL;
	blurjudger = NULL;
	
	imgView = new QScrollArea(this);
	imgLayout = new QVBoxLayout(this);
	blurjudger = new BlurJudger();
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


void Thumbnails::oncreateThumbnails( const QFileInfoList& imgList )
{
	QHBoxLayout* hlayout = NULL;
	QWidget* w = new QWidget(imgView);
	imgView->setWidget(w);
	w->setLayout(imgLayout);
	imgLayout->setAlignment(Qt::AlignTop);
	imgView->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	imgView->setWidgetResizable(true);
	QVector<QHBoxLayout*> hLayoutList;

	QFileInfoList  FileList;
	foreach(QFileInfo info, imgList)
	{
		bool flags = false;
		QString filename = 	QDir::toNativeSeparators(info.filePath());
		int m = blurjudger->Judge(filename, &flags);
		if(m != 0) {
			qDebug()<<"解析文件出错"<<filename;
			continue;
		}
		if(flags == false) 
		{
			FileList.push_back(info);
		}
	}

	for(int i = 0; i < FileList.length(); i++)
	{
		QHBoxLayout* hBox = new QHBoxLayout(w);
		hBox->setSpacing(3);
		hBox->setContentsMargins(0,0,0,0);
		hBox->setAlignment(Qt::AlignLeft|Qt::AlignTop);
		imgLayout->addLayout(hBox);
		//imgLayout->addStretch();
		hLayoutList.push_back(hBox);
	}
	int i = 0;
	QHBoxLayout* hLayout = NULL;
	foreach(QFileInfo info, FileList)
	{
		if (i%3 == 0)
		{
			hlayout = hLayoutList.at(i);
		}

		QLabel* lab = new QLabel(w);
		lab->setFixedWidth(150);
		lab->setFixedHeight(150);
		hlayout->addWidget(lab);
		lab->setProperty("img", info.filePath());
		lab->installEventFilter(this);

		QPixmap pic(info.filePath());
		lab->setPixmap(pic.scaled(150,150));
		if(i%3 == 2) imgLayout->addLayout(hlayout);

		i++;

	}
}

bool Thumbnails::eventFilter( QObject * o, QEvent * e )
{
	if(e->type() == QEvent::MouseButtonPress)
	{
		if (o->property("img").toString() != "")
		{
			//QPixmap pixmap(o->property("img").toString());
			//QLabel* l = qobject_cast<QLabel*>(o);
			//imgcon->setPixmap(pixmap.scaled(imgcon->width(), imgcon->height()));
			emit showPic(o->property("img").toString());
		}
	}
	return false;
}


