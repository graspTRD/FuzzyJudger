#ifndef JUDGEWORKER_H
#define JUDGEWORKER_H

#include <QObject>
#include <QDir>

class BlurJudger;

class JudgeWorker : public QObject
{
	Q_OBJECT
public:
	JudgeWorker(const QDir& dir, int force);

public slots: 	
	void doJudge();

signals:
	void finished();
	void itemFinished(const QString& item, bool result);
	void itemError(const QString& item, int code);

private: 
	QDir m_dir;
	int m_force;
};

#endif;