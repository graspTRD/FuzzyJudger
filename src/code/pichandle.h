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
	//bool eventFilter( QObject * o, QEvent * e );
private slots:
// 	void onselectBtnClicked();
// 	void onrenameBtnClicked();
// 	void onvalueChanged(int value);
protected:
	void paintEvent(QPaintEvent*);
	
private:
	void initUI();
// 	QString getImageDateInfo(const QString& imgName);
// 	QString getFileDate(const QString &fileName);
// 	void renameFile(const QString& fileName, const QString& newName);
// 	QByteArray getThumbnail(const QString &fileName);  // 缩略图
// 	QVariant readImage(const QString &fileName,const int &length,const int &quality = -1);
	//void showThumbnail();
private:
	ControllBar* controllbar;
	Thumbnails* thumbnails;
	PicCapture* piccapture;
// 	QLineEdit* floder;
// 	QToolButton* selectBtn;
// 	QToolButton* renameBtn;
// 	QProgressBar* progress;
// 	QLabel* progressText;
// 	QSlider* criticalpointSlider;  // 选取临界点
// 	QToolButton* refindBtn; // 选择模糊照片
// 	QScrollArea* imgView;
// 	QVBoxLayout* imgLayout; 
	//QLabel* imgcon;
};

#endif // PICHANDLE_H
