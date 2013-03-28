
#include "stdafx.h"
#include "stringutils.h"
#include <QUuid>

QString StringUtils::md5(const QString &text)
{
	QCryptographicHash hash(QCryptographicHash::Md5);
	hash.addData(text.toUtf8());
	QByteArray result = hash.result();
	return result.toHex();
}


QString StringUtils::ByteAarry2String(const QByteArray& arry)
{
	if((uchar)arry.at(0) ==  0xef && \
		(uchar)arry.at(1) == 0xbb && \
		(uchar)arry.at(0) == 0xbf )
	{//ÊÇUTF8±àÂë
		return QString::fromUtf8(arry.data(), arry.size());
	}

	return QString::fromLocal8Bit(arry.data(), arry.size());
}

QString StringUtils::firstLeft(QString srcString, const QChar partionChar, bool bKeepPartionChar)
{
	int pos = srcString.indexOf(partionChar);
	if(pos < 0) return srcString;
	return bKeepPartionChar? srcString.left(pos + 1): srcString.left(pos);
}


QString StringUtils::firstRight(QString srcString, const QChar partionChar, bool bKeepPartionChar)
{
	int pos = srcString.indexOf(partionChar);
	if(pos < 0) return srcString;
	int dstLength = srcString.length() - pos;
	return bKeepPartionChar? srcString.right(dstLength): srcString.right(dstLength - 1);
}


QString StringUtils::lastLeft(QString srcString, const QChar partionChar, bool bKeepPartionChar)
{
	int pos = srcString.lastIndexOf(partionChar);
	if(pos < 0) return srcString;
	return bKeepPartionChar? srcString.left(pos + 1): srcString.left(pos);
}

QString StringUtils::lastRight(QString srcString, const QChar partionChar, bool bKeepPartionChar)
{
	int pos = srcString.lastIndexOf(partionChar);
	if(pos < 0) return srcString;
	int dstLength = srcString.length() - pos;
	return bKeepPartionChar? srcString.right(dstLength): srcString.right(dstLength - 1);
}


QString StringUtils::generateUuid()
{
	QString id = QUuid::createUuid().toString();
	return id.mid(1, id.length() - 2);
}