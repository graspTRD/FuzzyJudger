#ifndef THUMBNAILS_H
#define THUMBNAILS_H

#include <QWidget>
#include <QPaintEvent>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QFileInfoList>
#include <QList>
#include <QCheckBox>
#include "blurjudger.h"

class Thumbnails : public QWidget
{
	Q_OBJECT
public:
	Thumbnails(QWidget *parent = NULL);
	~Thumbnails();
public slots:
	void oncreateThumbnails(const QFileInfoList& FileList);
	void ondealPic();

signals:
	void showPic(const QString&);

protected:
	void paintEvent(QPaintEvent*);
	bool eventFilter(QObject*, QEvent*);
	void resizeEvent(QResizeEvent *);

private slots:
	void slotItemFinished(const QString& picName, bool ret);
	void slotItemError(const QString& picName, int code);

private: 
	void onJudgePictures(const QDir& dir, int force);

private:
	QScrollArea* imgView;
	QVBoxLayout* imgLayout; 
	BlurJudger* blurjudger;
	QWidget* w;
	QList<QCheckBox*> imglist;
};

#endif // THUMBNAILS_H
