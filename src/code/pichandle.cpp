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
	
	
	
	this->setFixedHeight(600);
	this->setFixedWidth(450);
	initUI();
}

PicHandle::~PicHandle()
{

}

void PicHandle::initUI()
{
	floder = NULL;
	selectBtn = NULL;
	renameBtn = NULL;
	progress = NULL;
	progressText = NULL;
	criticalpointSlider = NULL;
	refindBtn = NULL;

	floder = new QLineEdit(this);
	selectBtn = new QToolButton(this);
	renameBtn = new QToolButton(this);
	progress = new QProgressBar(this);
	progressText = new QLabel(this);
	criticalpointSlider = new QSlider(Qt::Horizontal, this);
	refindBtn = new QToolButton(this);

	floder->setFixedHeight(25);
	floder->setFixedWidth(360);
	floder->setGeometry(10, 15, floder->width(), floder->height());
	floder->setPlaceholderText("选择/输入文件路径");
	floder->clearFocus();

	selectBtn->setObjectName("openBtn");
	selectBtn->setFixedHeight(30);
	selectBtn->setFixedWidth(60);
	selectBtn->setGeometry(380, 13, selectBtn->width(), selectBtn->height());
	selectBtn->setIconSize(QSize(28,28));
	selectBtn->setIcon(QIcon(":/floder.png"));
	connect(selectBtn, SIGNAL(clicked()), SLOT(onselectBtnClicked()));

	renameBtn->setObjectName("renameBtn");
	renameBtn->setFixedHeight(35);
	renameBtn->setFixedWidth(60);
	//renameBtn->setText("改名");
	renameBtn->setIconSize(QSize(30,30));
	renameBtn->setIcon(QIcon(":/ok.png"));
	renameBtn->setGeometry(380,53, renameBtn->width(), renameBtn->height());
	connect(renameBtn, SIGNAL(clicked()), SLOT(onrenameBtnClicked()));
	//renameBtn->setVisible(false);

	progress->setFixedHeight(40);
	progress->setFixedWidth(420);
	progress->setGeometry(10, 53, progress->width(), progress->height());
	progress->setVisible(false);
	connect(progress, SIGNAL(valueChanged(int)), SLOT(onvalueChanged(int)));

	progressText->setFixedHeight(40);
	progressText->setFixedWidth(60);
	progressText->setGeometry(405, 53, progressText->width(), progressText->height());
	progressText->setText("0 %");
	progressText->setVisible(false);


	criticalpointSlider->setFixedHeight(20);
	criticalpointSlider->setFixedWidth(360);
	criticalpointSlider->setGeometry(10, 110, criticalpointSlider->width(), criticalpointSlider->height());

	refindBtn->setFixedHeight(40);
	refindBtn->setFixedWidth(60);
	refindBtn->setGeometry(380, 100, refindBtn->width(), refindBtn->height());
}

void PicHandle::paintEvent(QPaintEvent* e)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	QWidget::paintEvent(e);
}

void PicHandle::onselectBtnClicked()
{
	QString directory = QFileDialog::getExistingDirectory(this,
		tr("Find Files"), QDir::currentPath());
	
	if (!directory.isEmpty()) {
		floder->setText(directory);
	}
}

void PicHandle::onrenameBtnClicked()
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
		QString newname = getImageDateInfo(filename).replace(":", "-");
		newname += " " + QString::number(i) + "." + suffix;
		renameFile(filename, newname);
		progress->setValue(i);
		float c = i/(float)filecount;
		progressText->setText(QString::number(c, 'f', 2) + " %");
		progressText->clear();
	}

}

void PicHandle::onvalueChanged( int value )
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


QString PicHandle::getFileDate(const QString &fileName)
{
	QFileInfo *info = new QFileInfo(fileName);
	QString date = info->created().toString(Qt::LocalDate);//.toString("yyyy-MM-dd");DateFormat::
	delete(info);
	return date;
}

QString PicHandle::getImageDateInfo( const QString& imgName )
{
	QString dateString = "";
	if (imgName.endsWith(".jpg", Qt::CaseInsensitive))
	{
		JpgHead jh;
		jh.processFile(imgName);
		QVariantMap imageInfo = jh.getExif();
		qDebug()<<imageInfo;
		if (!imageInfo.isEmpty())
		{
			dateString = imageInfo.value("DateTime").toString();
			return dateString;
		}
	}
	dateString = getFileDate(imgName);
	return dateString;
}

void PicHandle::renameFile( const QString& fileName, const QString& newName )
{
	QFileInfo fileinfo(fileName);
	QFile file(fileName);
	QString uu = fileName.left(fileName.length() - fileinfo.fileName().length());
	if (!file.exists()) return;
	QString tt= fileName.left(fileName.length() - fileinfo.fileName().length()) + "/" + newName;
	bool h = file.rename(fileName.left(fileName.length() - fileinfo.fileName().length()) + "/" + newName);
}

