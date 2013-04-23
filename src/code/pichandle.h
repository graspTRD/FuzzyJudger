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
#include "syspanel.h"
#include "viewer.h"

class PicHandle : public Viewer
{
	Q_OBJECT

public:
	PicHandle(QWidget *parent = NULL);
	~PicHandle();

protected:
	void paintEvent(QPaintEvent*);
	void resizeEvent(QResizeEvent* e);
	void mouseDoubleClickEvent(QMouseEvent* e);
private:
	void initUI();
private slots:
	void OnSysClose();
	void OnSysMin();
	void OnSysMaxMin();
private:
	bool m_bMaxed;
	QRect m_normalRect; // 正常状态窗口矩形
	SysPanel* syspanel;
	ControllBar* controllbar;
	Thumbnails* thumbnails;
	PicCapture* piccapture;
};

#endif // PICHANDLE_H
