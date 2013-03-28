#ifndef JPGHEAD_H
#define JPGHEAD_H
#include <QObject>
#include <QVariantMap>
#include "jhead.h"

class JpgHead : public QObject
{
	Q_OBJECT
public slots:
	bool processFile(QString FileName);//对图片进行初始化处理
	QByteArray getThumbnail(); //获取图片缩略图
	QVariantMap  getExif();

public:
	JpgHead();
	virtual ~JpgHead();

private:
	bool isProcessFile;
};
#endif