#ifndef SYSPANEL_H
#define SYSPANEL_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVariant>

class SysPanel : public QWidget
{
	Q_OBJECT

public:
	SysPanel(QWidget *parent = NULL);
	~SysPanel();
	QVariant getSysState(){ if(!m_maxBtn) return false; return m_maxBtn->property("normalBtn"); }
protected:
	void mousePressEvent(QMouseEvent*);
	void mouseMoveEvent(QMouseEvent*);
	void paintEvent(QPaintEvent*);
	
signals:
	void sysClose();
	void sysMin();
	void sysMaxMin();
	void sysSetting();
private slots:
	void OnSysClose();
	void OnSysMin();
	void OnSysMaxMin();
	void OnSysReducted();
	void OnSysMax();
private:
	QHBoxLayout* m_syspanel;

	QPushButton* m_closeBtn;
	QPushButton* m_maxBtn;
	QPushButton* m_minBtn;
};

#endif // SYSPANEL_H
