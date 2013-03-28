#ifndef PICCAPTURE_H
#define PICCAPTURE_H

#include <QWidget>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QLabel>

class PicCapture : public QWidget
{
	Q_OBJECT

public:
	PicCapture(QWidget *parent = NULL);
	~PicCapture();
protected:
	void paintEvent(QPaintEvent*);
	void resizeEvent(QResizeEvent *);
public slots:
	void onshowPic(const QString& file); 
private:
	QLabel* picLabel;
};

#endif // PICCAPTURE_H
