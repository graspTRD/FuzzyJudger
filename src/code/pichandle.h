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
class PicHandle : public QWidget
{
	Q_OBJECT

public:
	PicHandle(QWidget *parent = NULL);
	~PicHandle();

private slots:
	void onselectBtnClicked();
	void onrenameBtnClicked();
	void onvalueChanged(int value);
protected:
	void paintEvent(QPaintEvent*);
private:
	void initUI();
	QString getImageDateInfo(const QString& imgName);
	QString getFileDate(const QString &fileName);
	void renameFile(const QString& fileName, const QString& newName);
private:
	QLineEdit* floder;
	QToolButton* selectBtn;
	QToolButton* renameBtn;
	QProgressBar* progress;
	QLabel* progressText;
	QSlider* criticalpointSlider;  // 选取临界点
	QToolButton* refindBtn; // 选择模糊照片
};

#endif // PICHANDLE_H
