#ifndef THUMBNAILS_H
#define THUMBNAILS_H

#include <QWidget>
#include <QPaintEvent>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QFileInfoList>
#include "blurjudger.h"

class Thumbnails : public QWidget
{
	Q_OBJECT

public:
	Thumbnails(QWidget *parent = NULL);
	~Thumbnails();
protected:
	void paintEvent(QPaintEvent*);
	bool eventFilter(QObject*, QEvent*);
    void resizeEvent(QResizeEvent *);
signals:
	void showPic(const QString&);
public slots:
	void oncreateThumbnails(const QFileInfoList& FileList);
private:
	QScrollArea* imgView;
	QVBoxLayout* imgLayout; 
	BlurJudger* blurjudger;
};

#endif // THUMBNAILS_H
