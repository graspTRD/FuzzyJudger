#ifndef CONTROLLBAR_H
#define CONTROLLBAR_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QPaintEvent>
#include <QToolButton>
#include <QSlider>
#include <QFileInfoList>

class ControllBar : public QWidget
{
	Q_OBJECT

public:
	ControllBar(QWidget *parent = NULL);
	~ControllBar();
protected:
	void paintEvent(QPaintEvent* e);
signals:
	void createThumbnails(const QDir& dir, int);
	void dealPic();
	void sliderValueChanged(int);
public slots:
	void onsetpiccount(int);
	void onpicstepchanged(int);
	void onpicdealfinished();
private slots:
	void onselectBtnClicked();
	void onrenameBtnClicked();
	void onvalueChanged( int value );
	void onrefindBtnClicked();
	void ondealBtnClicked();
	void onslidervalueChanged(int);
private:
	QLineEdit* floder;
	QToolButton* selectBtn;
	QLabel* seltips;
	
	QToolButton* renameBtn;
	QLabel* renametips;
	QProgressBar* progress;
	QLabel* progressText;
	QSlider* criticalpointSlider;  // 选取临界点
	QLabel* slidertext;
	QToolButton* refindBtn; // 选择模糊照片
	QLabel* refindLabel;
	QToolButton* dealBtn;
	QLabel* dealLabel;
};

#endif // CONTROLLBAR_H
