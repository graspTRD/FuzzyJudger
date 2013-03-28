#include "stdafx.h"
#include "judgeworker.h"
#include "blurjudger.h"

JudgeWorker::JudgeWorker(const QDir& dir, int force)
	:m_dir(dir), m_force(force)
{

}

void JudgeWorker::doJudge()
{
	QStringList filters;
	filters<<"*.BMP"<<"*.JPG"<<"*.JPEG"<<"*.PNG"<<"*.GIF";
	m_dir.setNameFilters(filters);
	
	BlurJudger judger(m_force);
	QStringList picList = m_dir.entryList();
	foreach(QString picName,  picList)
	{
		bool flag(true);
		int ret = judger.Judge(picName, &flag);
		if(ret) 
		{
			emit itemError(picName, ret);
			emit itemFinished(picName, true);
		}
		else
		{ 
			emit itemFinished(picName, flag);
		}
	}
	emit finished();
}

