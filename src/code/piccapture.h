#ifndef PICCAPTURE_H
#define PICCAPTURE_H

#include <QWidget>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include "picdetail.h"
class GraphicsView;
class PicCapture : public QWidget
{
	Q_OBJECT

public:
	PicCapture(QWidget *parent = NULL);
	~PicCapture();
protected:
	void paintEvent(QPaintEvent*);
	void resizeEvent(QResizeEvent *);
	bool eventFilter(QObject* o, QEvent* e);
public slots:
	void onshowPic(const QString& file); 
private slots:
	void onExc();
	void ondbclicked();
private:
	QLabel* picLabel;
	QString filename;
	PicDetail* picdetail;

	GraphicsView* view;
	QGraphicsScene* scene;
	QGraphicsPixmapItem* pixmapItem;
};

class GraphicsView: public QGraphicsView
{
	Q_OBJECT
public:
	GraphicsView(QWidget* parent = NULL);
	~GraphicsView();

signals:
 	void dbclicked();
// 	void onMouseMoveEvent(QMouseEvent *event);

protected:
	void mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // PICCAPTURE_H
