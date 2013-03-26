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
	void createThumbnails(const QFileInfoList& FileList);
private slots:
	void onselectBtnClicked();
	void onrenameBtnClicked();
	void onvalueChanged( int value );
	void onrefindBtnClicked();
private:
	QLineEdit* floder;
	QToolButton* selectBtn;
	QToolButton* renameBtn;
	QProgressBar* progress;
	QLabel* progressText;
	QSlider* criticalpointSlider;  // ѡȡ�ٽ��
	QToolButton* refindBtn; // ѡ��ģ����Ƭ
};

#endif // CONTROLLBAR_H
