#ifndef PICDETAIL_H
#define PICDETAIL_H

#include <QWidget>

class PicDetail : public QWidget
{
	Q_OBJECT

public:
	PicDetail(QWidget *parent = NULL);
	~PicDetail();
	void showPic(const QString& img);
signals:
	void exc();
private:
	QLabel* pic; 
};

#endif // PICDETAIL_H
