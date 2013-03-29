#ifndef PICHANDLE_H
#define PICHANDLE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QPaintEvent>
#include <QToolButton>
#include <QSlider>
#include <QScrollArea>

#include "controllbar.h"
#include "thumbnails.h"
#include "piccapture.h"

class PicHandle : public QWidget
{
	Q_OBJECT

public:
	PicHandle(QWidget *parent = NULL);
	~PicHandle();

protected:
	void paintEvent(QPaintEvent*);
	
private:
	void initUI();
private:
	ControllBar* controllbar;
	Thumbnails* thumbnails;
	PicCapture* piccapture;

};

#endif // PICHANDLE_H
