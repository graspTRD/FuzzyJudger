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
// 	floder = NULL;
// 	selectBtn = NULL;
// 	renameBtn = NULL;
// 	progress = NULL;
// 	progressText = NULL;
// 	criticalpointSlider = NULL;
// 	refindBtn = NULL;
// 	imgView = NULL;
// 	imgLayout = NULL;
	//imgcon = NULL;

	controllbar->setGeometry(10,10, 450, 150);
	connect(controllbar, SIGNAL(createThumbnails(const QFileInfoList&)), thumbnails, SLOT(oncreateThumbnails(const QFileInfoList&)));

	thumbnails->setGeometry(470, 10, width() - 480, height() - 20);
	piccapture->setGeometry(10, 160, 450, height() - 170);
	connect(thumbnails, SIGNAL(showPic(const QString&)), piccapture, SLOT(onshowPic(const QString&)));
	connect(controllbar, SIGNAL(dealPic()), thumbnails, SLOT(ondealPic()));
// 	floder = new QLineEdit(this);
// 	selectBtn = new QToolButton(this);
// 	renameBtn = new QToolButton(this);
// 	progress = new QProgressBar(this);
// 	progressText = new QLabel(this);
// 	criticalpointSlider = new QSlider(Qt::Horizontal, this);
// 	refindBtn = new QToolButton(this);
// 	imgView = new QScrollArea(this);
// 	imgLayout = new QVBoxLayout(this);
	//imgcon = new QLabel(this);

// 	floder->setFixedHeight(25);
// 	floder->setFixedWidth(360);
// 	floder->setGeometry(10, 15, floder->width(), floder->height());
// 	floder->setPlaceholderText("选择/输入文件路径");
// 	floder->clearFocus();
// 
// 	selectBtn->setObjectName("openBtn");
// 	selectBtn->setFixedHeight(30);
// 	selectBtn->setFixedWidth(60);
// 	selectBtn->setGeometry(380, 13, selectBtn->width(), selectBtn->height());
// 	selectBtn->setIconSize(QSize(28,28));
// 	selectBtn->setIcon(QIcon(":/floder.png"));
// 	connect(selectBtn, SIGNAL(clicked()), SLOT(onselectBtnClicked()));
// 
// 	renameBtn->setObjectName("renameBtn");
// 	renameBtn->setFixedHeight(35);
// 	renameBtn->setFixedWidth(60);
// 	//renameBtn->setText("改名");
// 	renameBtn->setIconSize(QSize(30,30));
// 	renameBtn->setIcon(QIcon(":/ok.png"));
// 	renameBtn->setGeometry(380,53, renameBtn->width(), renameBtn->height());
// 	connect(renameBtn, SIGNAL(clicked()), SLOT(onrenameBtnClicked()));
// 	//renameBtn->setVisible(false);
// 
// 	progress->setFixedHeight(40);
// 	progress->setFixedWidth(420);
// 	progress->setGeometry(10, 53, progress->width(), progress->height());
// 	progress->setVisible(false);
// 	connect(progress, SIGNAL(valueChanged(int)), SLOT(onvalueChanged(int)));
// 
// 	progressText->setFixedHeight(40);
// 	progressText->setFixedWidth(60);
// 	progressText->setGeometry(405, 53, progressText->width(), progressText->height());
// 	progressText->setText("0 %");
// 	progressText->setVisible(false);
// 
// 
// 	criticalpointSlider->setFixedHeight(20);
// 	criticalpointSlider->setFixedWidth(360);
// 	criticalpointSlider->setGeometry(10, 110, criticalpointSlider->width(), criticalpointSlider->height());
// 
// 	refindBtn->setFixedHeight(40);
// 	refindBtn->setFixedWidth(60);
// 	refindBtn->setGeometry(380, 100, refindBtn->width(), refindBtn->height());
	//////////////////////////////////////////////////////////////////////////
// 	imgView->setFixedHeight(this->height() - 30);
// 	imgView->setFixedWidth(width() - 500);
// 	imgView->setGeometry(480, 10, imgView->width(), imgView->height());

// 	imgcon->setFixedHeight(450);
// 	imgcon->setFixedWidth(450);
// 	imgcon->setGeometry(10,200, imgcon->width(), imgcon->height());

}

void PicHandle::paintEvent(QPaintEvent* e)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	QWidget::paintEvent(e);
}

// void PicHandle::onselectBtnClicked()
// {
// 	QString directory = QFileDialog::getExistingDirectory(this,
// 		tr("Find Files"), QDir::currentPath());
// 	
// 	if (!directory.isEmpty()) {
// 		floder->setText(directory);
// 	}
// }

// void PicHandle::onrenameBtnClicked()
// {
// 	if(floder->text() == "") return;
// 	QDir dir(floder->text());
// 	if (!dir.exists()) return;
// 
// 	renameBtn->setVisible(false);
// 	progress->setVisible(true);
// 	progressText->setVisible(true);
// 
// 	QStringList filters;
// 	filters<<"*.BMP"<<"*.JPG"<<"*.JPEG"<<"*.PNG"<<"*.GIF";
// 	dir.setNameFilters(filters);
// 
// 	QFileInfoList FileList = dir.entryInfoList();
// 	//qDebug()<<FileList;
// 	int i = 0;
// 	int filecount = FileList.length();
// 	progress->setRange(0, filecount);
// 	progress->setValue(0);
// 	progress->setTextVisible(true);
// 	foreach(QFileInfo info, FileList)
// 	{
// 		i++;
// 		QString suffix = info.completeSuffix();
// 		QString filename = info.filePath();
// 		QString newname = getImageDateInfo(filename).replace(":", "-");
// 		newname += " " + QString::number(i) + "." + suffix;
// 		renameFile(filename, newname);
// 		progress->setValue(i);
// 		float c = i/(float)filecount;
// 		progressText->setText(QString::number(c, 'f', 2) + " %");
// 		progressText->clear();
// 	}
// 
// 	//显示缩略图
// 	showThumbnail();
// }
// 
// void PicHandle::showThumbnail()
// {
// 	if(floder->text() == "") return;
// 	QDir dir(floder->text());
// 	if (!dir.exists()) return;
// 
// 	renameBtn->setVisible(false);
// 	progress->setVisible(true);
// 	progressText->setVisible(true);
// 
// 	QStringList filters;
// 	filters<<"*.BMP"<<"*.JPG"<<"*.JPEG"<<"*.PNG"<<"*.GIF";
// 	dir.setNameFilters(filters);
// 
// 	QFileInfoList FileList = dir.entryInfoList();
// 	
// 	QHBoxLayout* hlayout = NULL;
// 	QWidget* w = new QWidget(imgView);
// 	imgView->setWidget(w);
// 	w->setLayout(imgLayout);
// 	imgLayout->setAlignment(Qt::AlignTop);
// 	imgView->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
// 	imgView->setWidgetResizable(true);
// 	QVector<QHBoxLayout*> hLayoutList;
// 	for(int i = 0; i < FileList.length(); i++)
// 	{
//  		QHBoxLayout* hBox = new QHBoxLayout(w);
// 		hBox->setSpacing(3);
// 		hBox->setContentsMargins(0,0,0,0);
// 		hBox->setAlignment(Qt::AlignLeft|Qt::AlignTop);
// 		imgLayout->addLayout(hBox);
// 		//imgLayout->addStretch();
// 		hLayoutList.push_back(hBox);
// 	}
// 	int i = 0;
// 	QHBoxLayout* hLayout = NULL;
// 	foreach(QFileInfo info, FileList)
// 	{
// 		if (i%3 == 0)
// 		{
// 			hlayout = hLayoutList.at(i);
// 		}
// 
// 		QLabel* lab = new QLabel(w);
// 		lab->setFixedWidth(150);
// 		lab->setFixedHeight(150);
// 		hlayout->addWidget(lab);
// 		lab->setProperty("img", info.filePath());
// 		lab->installEventFilter(this);
// 
// 		QPixmap pic(info.filePath());
// 		lab->setPixmap(pic.scaled(150,150));
// 		if(i%3 == 2) imgLayout->addLayout(hlayout);
// 		
// 		i++;
// 
// 	}
// }

// void PicHandle::onvalueChanged( int value )
// {
// 	if(value == progress->maximum())
// 	{
//  		progress->setVisible(false);
//  		progress->setValue(0);
//  		progressText->setVisible(false);
//  		progressText->setText("0 %");
// 		renameBtn->setVisible(true);
// 	}
// }


// bool PicHandle::eventFilter( QObject * o, QEvent * e )
// {
// 	if(e->type() == QEvent::MouseButtonPress)
// 	{
// 		if (o->property("img").toString() != "")
// 		{
// 			QPixmap pixmap(o->property("img").toString());
// 			//QLabel* l = qobject_cast<QLabel*>(o);
// 			imgcon->setPixmap(pixmap.scaled(imgcon->width(), imgcon->height()));
// 		}
// 	}
// 	return false;
// }


