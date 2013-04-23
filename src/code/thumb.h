#ifndef THUMB_H
#define THUMB_H

#include <QWidget>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QLabel>

class Thumb : public QWidget
{
	Q_OBJECT

public:
	Thumb(QWidget *parent = NULL);
	~Thumb();
	void setImg(const QString& img);
private:
	QVBoxLayout* layout;
	QLabel* pic;
	QCheckBox* check;
};

#endif // THUMB_H
