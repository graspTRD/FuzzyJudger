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
	bool isDbclickHead(const QPoint& pt);  // 判断双击区域是否有效
	bool isclickeAvalibleArea(const QPoint& pt); // 判断单击位置是否适合移动窗体

protected:
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
	void paintEvent(QPaintEvent* e);
public:
   
private:
    static const int borderWidth = 4;  // 边框宽度
    QPoint m_ptPressGlobal;
    enum_Direction m_eDirection;
    bool m_bMaxed;  // 是否最大化
    QPoint m_recordPt; // 记录单击移动位置
	bool isMoveable;
};

#endif 
