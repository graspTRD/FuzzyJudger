#include "stdafx.h"
#include "thumb.h"
#include "fileutils.h"
#include <QPixmap>

Thumb::Thumb(QWidget *parent) : QWidget(parent)
{
	layout = NULL;
	pic = NULL;
	check = NULL;

	layout = new QVBoxLayout();
	layout->setObjectName("vbox");
	layout->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
	layout->setContentsMargins(0,0,0,0);
	
	
}

Thumb::~Thumb()
{

}

void Thumb::setImg( const QString& img )
{
	pic = new QLabel(this);
	pic->setFixedWidth(width());
	pic->setFixedHeight(height());

	check = new QCheckBox(this);
	
	this->setLayout(layout);
	layout->addWidget(pic,0);
	layout->addWidget(check,1);

	

	QPixmap pp;
	pp.loadFromData(FileUtils::getThumbnail(img));
	pic->setPixmap(pp.scaled(width(), height()));
	//vbox->addWidget(checkbox);
}
