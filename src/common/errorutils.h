
#ifndef ERROR_H
#define ERROR_H
#include<QObject>
#include <QString>

 class Error: public QObject
 {
	 Q_OBJECT
 public:
	 Error();
	 Error(QString error);
	~Error();

	static void throwError(const QString &error);
	static void throwArgumentNull(const QString &paramName);
	static void checkAndThrowError(bool state, QString& error);
	static void showError(const QString &error, const QString& title = QString("����"));
	QString error() const;
	
#ifdef _DEBUG
	static void debugError(QString& error);//������󵽿���̨
	static void Error::debugError(const char* error);
#endif

 protected:
	 QString errorInfo;
 };

#endif