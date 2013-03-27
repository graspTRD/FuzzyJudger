#include "stdafx.h"
#include "thumbnails.h"
#include <QStyleOption>
#include <QPainter>
#include <QLabel>
#include <QCheckBox>
#include <windows.h>
#include <shellapi.h>

Thumbnails::Thumbnails(QWidget *parent)	: QWidget(parent)
{
	imgView = NULL;
	imgLayout = NULL;
	blurjudger = NULL;
	blurjudger = new BlurJudger();


	imgView = new QScrollArea(this);
	imgLayout = new QVBoxLayout(imgView);
	
	w = new QWidget(imgView);
	w->setLayout(imgLayout);
	imgLayout->setAlignment(Qt::AlignTop);
	imgView->setWidget(w);
 	imgView->setContentsMargins(0,0,0,0);
 	imgLayout->setSpacing(0);
 	imgLayout->setContentsMargins(0,0,0,0);
 	imgLayout->setAlignment(Qt::AlignLeft|Qt::AlignTop);
	imgView->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	imgView->setWidgetResizable(true);

	imgLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);

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
	imgView->setObjectName("aa");
	//imgView->setStyleSheet("QScrollArea#aa{background-color:red}");
}


void Thumbnails::oncreateThumbnails( const QFileInfoList& imgList )
{

	imglist.clear();
	QHBoxLayout* hlayout = NULL;
	QVector<QHBoxLayout*> hLayoutList;

	QFileInfoList  FileList;
// 	foreach(QFileInfo info, imgList)
// 	{
// 		bool flags = false;
// 		QString filename = 	QDir::toNativeSeparators(info.filePath());
// 		int m = blurjudger->Judge(filename, &flags);
// 		if(m != 0) {
// 			qDebug()<<"�����ļ�����"<<filename;
// 			continue;
// 		}
// 		if(flags == false) 
// 		{
// 			FileList.push_back(info);
// 		}
// 	}
	FileList = imgList;
	int k = 0;
	k = FileList.length()%3==0 ? FileList.length()/3:FileList.length()/3 + 1;
	for(int i = 0; i < k; i++)
	{
		QHBoxLayout* hBox = new QHBoxLayout(w);
		hBox->setSpacing(3);
		hBox->setContentsMargins(3,0,0,0);
		//hBox->setAlignment(Qt::AlignLeft|Qt::AlignTop);
		imgLayout->insertLayout(i,hBox);
		//imgLayout->addStretch();
		hLayoutList.push_back(hBox);
	}
	int i = 0;
	foreach(QFileInfo info, FileList)
	{
		if (i%3 == 0)
		{
			hlayout = hLayoutList.at(i/3);
		}
		QVBoxLayout* vbox = new QVBoxLayout(w);
		vbox->setAlignment(Qt::AlignCenter);
		QCheckBox* checkbox = new QCheckBox(w);
		checkbox->setProperty("imgname", info.filePath());
		imglist.insert(0, checkbox);
		QLabel* lab = new QLabel(w);
		lab->setFixedWidth(imgView->width()/3 - 15);
		lab->setFixedHeight(imgView->width()/3 - 15);
		vbox->addWidget(lab);
		lab->setProperty("img", info.filePath());
		lab->installEventFilter(this);

		QPixmap pp;
		pp.loadFromData(FileUtils::getThumbnail(info.filePath()));
		lab->setPixmap(pp.scaled(imgView->width()/3 - 15,imgView->width()/3 - 15));
		vbox->addWidget(checkbox);

		hlayout->addLayout(vbox);

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
			emit showPic(o->property("img").toString());
		}
	}
	return false;
}

void Thumbnails::ondealPic()
{

	SHFILEOPSTRUCT FileOp={0}; 
	FileOp.fFlags = FOF_ALLOWUNDO |   //����Żػ���վ
		FOF_NOCONFIRMATION; //������ȷ�϶Ի���
	FileOp.pFrom = L"G:\\Users\\shenghai\\Desktop\\bbbcc";//(LPCWSTR)ss.toStdString().c_str(); 
	FileOp.pTo = NULL;      //һ��Ҫ��NULL
	FileOp.wFunc = FO_DELETE;    //ɾ������
	bool nOk = SHFileOperation(&FileOp);
	if(nOk)
		qDebug()<<"ɾ��ʧ��";
	else
		qDebug()<<"ɾ���ɹ�";
	/*
	foreach(QCheckBox* box, imglist)
	{
		QString ss = box->property("imgname").toString();

		SHFILEOPSTRUCT FileOp={0}; 
		FileOp.fFlags = FOF_ALLOWUNDO |   //����Żػ���վ
			FOF_NOCONFIRMATION; //������ȷ�϶Ի���
		FileOp.pFrom = L"G:\\Users\\shenghai\\Desktop\\bbbcc\0";//(LPCWSTR)ss.toStdString().c_str(); 
		FileOp.pTo = NULL;      //һ��Ҫ��NULL
		FileOp.wFunc = FO_DELETE;    //ɾ������
		//return SHFileOperation(&FileOp) == 0; 



		//char strSrc[]= ss.toStdString().c_str();//"d:\\Vb\0";//Դ�ļ�·��;
// 		char strDst[]="d:\\Vb1\0";//Ŀ���ļ�·��;
// 
// 		SHFILEOPSTRUCT FileOp;//����SHFILEOPSTRUCT�ṹ����;
// 		//FileOp.hwnd=this->m_hWnd;
// 		FileOp.wFunc=FO_DELETE; //ִ���ļ�ɾ������;
// 		FileOp.pFrom = (LPCWSTR)ss.toStdString().c_str();
// 		FileOp.pTo = NULL;//(LPCWSTR)ss.toStdString().c_str();
// 		FileOp.fFlags=FOF_ALLOWUNDO;//�˱�־ʹɾ���ļ����ݵ�Windows����վ
// 		FileOp.hNameMappings=NULL;
// 		//FileOp.lpszProgressTitle=strTitle;
// 		//���￪ʼɾ���ļ�
		bool nOk = SHFileOperation(&FileOp);
		if(nOk)
			qDebug()<<"ɾ��ʧ��";
		else
			qDebug()<<"ɾ���ɹ�";
	}
	*/
			 /*foreach(QCheckBox* box, imglist)
	{
		if (box->isChecked())
		{
			//QFile::remove(box->property("imgname").toString(),);
			
			//	qDebug()<<box->property("imgname").toString();
		}
	}*/
}


