#ifndef VIEWER_H
#define VIEWER_H

#include <QWidget>
#include <QMouseEvent>

class Viewer : public QWidget
{
    Q_OBJECT

public:
    enum ScreenOrientation
    {
        ScreenOrientationLockPortrait,
        ScreenOrientationLockLandscape,
        ScreenOrientationAuto
    };
    enum enum_Direction
    {
        eNone = 0,
        eTop,
        eBottom,
        eRight,
        eLeft,
        eTopRight,
        eBottomLeft,
        eRightBottom,
        eLeftTop
    };
    explicit Viewer(QWidget *parent = 0);
    virtual ~Viewer();

    void setOrientation(ScreenOrientation orientation);

    void showExpanded();
	void SetCursorStyle(enum_Direction direction);
	void SetDrayMove(int nXGlobal,int nYGlobal,enum_Direction direction,QPoint pos);
	enum_Direction PointValid(int x,int y);
	bool isDbclickHead(const QPoint& pt);  // �ж�˫�������Ƿ���Ч
	bool isclickeAvalibleArea(const QPoint& pt); // �жϵ���λ���Ƿ��ʺ��ƶ�����

protected:
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
	void paintEvent(QPaintEvent* e);
public:
   
private:
    static const int borderWidth = 4;  // �߿���
    QPoint m_ptPressGlobal;
    enum_Direction m_eDirection;
    bool m_bMaxed;  // �Ƿ����
    QPoint m_recordPt; // ��¼�����ƶ�λ��
	bool isMoveable;
};

#endif 
