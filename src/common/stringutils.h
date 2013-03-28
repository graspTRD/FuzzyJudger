#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <QObject>

class StringUtils:public QObject
{
	Q_OBJECT
public:
	static QString md5(const QString &text);
	static QString ByteAarry2String(const QByteArray& arry);
	static QString firstLeft(QString srcString, const QChar partionChar, bool bKeepPartionChar = false);
	static QString firstRight(QString srcString, const QChar partionChar, bool bKeepPartionChar = false);
	static QString lastLeft(QString srcString, const QChar partionChar, bool bKeepPartionChar = false);
	static QString lastRight(QString srcString, const QChar partionChar, bool bKeepPartionChar = false);
	static QString generateUuid();
};


#endif;