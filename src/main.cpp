#include "stdafx.h"
#include "code/pichandle.h"

#include <QtGui/QApplication>
#include <QResource>
#include <QTextCodec>
#include <QString>
#include <QDir>
#include <QTranslator>
#include <QFile>
#include <QGridLayout>
#include <QtSql/QSqlDatabase>
#include <QStringList>
#include <QDebug>
#include <QIODevice>
#include <QDataStream>
#include <QImage>

void initqt_zh_CN();
void initStyle();

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	initqt_zh_CN();
	initStyle();
  	PicHandle w;
  	w.show();
	return app.exec();
}

void initqt_zh_CN()  // 中文支持
{
	QFont font("楷体",10);
	QApplication::setFont(font);
	QTextCodec *codec = QTextCodec::codecForName("System");
	QTextCodec::setCodecForLocale(codec);
	QTextCodec::setCodecForCStrings(codec);
	QTextCodec::setCodecForTr(codec);
	QTranslator *translator = new QTranslator(); 
	translator->load("qt_zh_CN", QApplication::applicationDirPath(), NULL, NULL);
	//qApp->installTranslator(translator);
}

void initStyle()  // 加载qss 
{
	QString exeFileName = QApplication::applicationFilePath();
	QFileInfo kk(exeFileName);
	QString apppath = kk.canonicalPath(); 
	QDir::setCurrent(apppath/*QDir::currentPath()*/);//QApplication::applicationDirPath()
	QFile file(":/skin.qss");
	file.open(QFile::ReadOnly);
	QString style = QString(file.readAll());
	qApp->setStyleSheet(style);
	file.close();
}
