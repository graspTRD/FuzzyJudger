#include "stdafx.h"
#include "controllbar.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QStyleOption>
#include <QPainter>
#include "common/fileutils.h"

ControllBar::ControllBar(QWidget *parent) : QWidget(parent)
{
	floder = NULL;
	selectBtn = NULL;
	renameBtn = NULL;
	progress = NULL;
	progressText = NULL;
	criticalpointSlider = NULL;
	refindBtn = NULL;
	dealBtn = NULL;
	seltips = NULL;
	renametips = NULL;

	floder = new QLineEdit(this);
	selectBtn = new QToolButton(this);
	renameBtn = new QToolButton(this);
	progress = new QProgressBar(this);
	progressText = new QLabel(this);
	criticalpointSlider = new QSlider(Qt::Horizontal, this);
	refindBtn = new QToolButton(this);
	dealBtn = new QToolButton(this);

	floder->setFixedHeight(25);
	floder->setFixedWidth(360);
	floder->setGeometry(10, 10, floder->width(), floder->height());
	floder->setPlaceholderText("选择/输入文件路径");
	floder->clearFocus();

	selectBtn->setObjectName("openBtn");
	selectBtn->setFixedHeight(30);
	selectBtn->setFixedWidth(60);
	selectBtn->setGeometry(380, 10, selectBtn->width(), selectBtn->height());
// 	selectBtn->setIconSize(QSize(28,28));
// 	selectBtn->setIcon(QIcon(":/floder.png"));
	connect(selectBtn, SIGNAL(clicked()), SLOT(onselectBtnClicked()));

	seltips = new QLabel(selectBtn);
	seltips->setFixedWidth(selectBtn->width() - 15);
	seltips->setFixedHeight(selectBtn->height() - 12);
	seltips->setPixmap(QPixmap(":/selfloder.png").scaled(seltips->width(),seltips->height()));
	seltips->setGeometry(selectBtn->width()/2 - seltips->width()/2, selectBtn->height()/2 - seltips->height()/2, seltips->width(), seltips->height());
// 	selpic = new QLabel(selectBtn);
// 	selpic->setPixmap(QPixmap(":/floder.png").scaled(30,30));

	renameBtn->setObjectName("renameBtn");
	renameBtn->setFixedHeight(35);
	renameBtn->setFixedWidth(430);
	//renameBtn->setText("改名");
// 	renameBtn->setIconSize(QSize(30,30));
// 	renameBtn->setIcon(QIcon(":/ok.png"));
	renameBtn->setGeometry(8,43, renameBtn->width(), renameBtn->height());
	connect(renameBtn, SIGNAL(clicked()), SLOT(onrenameBtnClicked()));

	renametips = new QLabel(renameBtn);
	renametips->setFixedWidth(100);
	renametips->setFixedHeight(20);
	renametips->setPixmap(QPixmap(":/rename.png").scaled(renametips->width(),renametips->height()));
	renametips->setGeometry(renameBtn->width()/2 - renametips->width()/2, renameBtn->height()/2 - renametips->height()/2, seltips->width(), seltips->height());
	// 	
	//renameBtn->setVisible(false);

	progress->setFixedHeight(40);
	progress->setFixedWidth(420);
	progress->setGeometry(10, 43, progress->width(), progress->height());
	progress->setVisible(false);
	connect(progress, SIGNAL(valueChanged(int)), SLOT(onvalueChanged(int)));

	progressText->setFixedHeight(40);
	progressText->setFixedWidth(60);
	progressText->setGeometry(405, 43, progressText->width(), progressText->height());
	progressText->setText("0 %");
	progressText->setVisible(false);


	criticalpointSlider->setFixedHeight(20);
	criticalpointSlider->setFixedWidth(260);
	criticalpointSlider->setGeometry(10, 110, criticalpointSlider->width(), criticalpointSlider->height());

	refindBtn->setFixedHeight(40);
	refindBtn->setFixedWidth(60);
	refindBtn->setGeometry(280, 100, refindBtn->width(), refindBtn->height());
	connect(refindBtn, SIGNAL(clicked()), SLOT(onrefindBtnClicked()));
	refindLabel = NULL;
	refindLabel = new QLabel(refindBtn);
	refindLabel->setText("检索");


	dealBtn->setFixedHeight(40);
	dealBtn->setFixedWidth(60);
	dealBtn->setGeometry(380, 100, dealBtn->width(), dealBtn->height());
	connect(dealBtn, SIGNAL(clicked()), SLOT(ondealBtnClicked()));

	dealLabel = NULL;
	dealLabel = new QLabel(dealBtn);
	dealLabel->setText("删除");
}

ControllBar::~ControllBar()
{

}

void ControllBar::paintEvent(QPaintEvent* e)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	QWidget::paintEvent(e);
}

void ControllBar::onselectBtnClicked()
{
	QString directory = QFileDialog::getExistingDirectory(this,
		tr("Find Files"), QDir::currentPath());

	if (!directory.isEmpty()) {
		floder->setText(directory);
	}
}

void ControllBar::onrenameBtnClicked()
{
	if(floder->text() == "") return;
	QDir dir(floder->text());
	if (!dir.exists()) return;

	renameBtn->setVisible(false);
	progress->setVisible(true);
	progressText->setVisible(true);

	QStringList filters;
	filters<<"*.BMP"<<"*.JPG"<<"*.JPEG"<<"*.PNG"<<"*.GIF";
	dir.setNameFilters(filters);

	QFileInfoList FileList = dir.entryInfoList();
	//qDebug()<<FileList;
	int i = 0;
	int filecount = FileList.length();
	progress->setRange(0, filecount);
	progress->setValue(0);
	progress->setTextVisible(true);
	foreach(QFileInfo info, FileList)
	{
		i++;
		QString suffix = info.completeSuffix();
		QString filename = info.filePath();
		QString newname = FileUtils::getImageDateInfo(filename).replace(":", "-");
		newname += " " + QString::number(i) + "." + suffix;
		FileUtils::renameimgFile(filename, newname);
		progress->setValue(i);
		float c = i/(float)filecount;
		progressText->setText(QString::number(c, 'f', 2) + " %");
		progressText->clear();
	}

	//显示缩略图
	//showThumbnail();
}

void ControllBar::onvalueChanged( int value )
{
	if(value == progress->maximum())
	{
		progress->setVisible(false);
		progress->setValue(0);
		progressText->setVisible(false);
		progressText->setText("0 %");
		renameBtn->setVisible(true);
	}
}

void ControllBar::onrefindBtnClicked()
{
	if(floder->text() == "") return;
	QDir dir(floder->text());
	if (!dir.exists()) return;

	QStringList filters;
	filters<<"*.BMP"<<"*.JPG"<<"*.JPEG"<<"*.PNG"<<"*.GIF";
	dir.setNameFilters(filters);

	QFileInfoList FileList = dir.entryInfoList();
	emit createThumbnails(FileList);
}

void ControllBar::ondealBtnClicked()
{
	emit dealPic();
}
