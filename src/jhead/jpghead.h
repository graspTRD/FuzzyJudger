#ifndef JPGHEAD_H
#define JPGHEAD_H
#include <QObject>
#include <QVariantMap>
#include "jhead.h"

class JpgHead : public QObject
{
	Q_OBJECT
public slots:
	bool processFile(QString FileName);//��ͼƬ���г�ʼ������
	QByteArray getThumbnail(); //��ȡͼƬ����ͼ
	QVariantMap  getExif();

public:
	JpgHead();
	virtual ~JpgHead();

private:
	bool isProcessFile;
};
#endif