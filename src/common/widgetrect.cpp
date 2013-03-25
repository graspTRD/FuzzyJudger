#include "stdafx.h"
#include "widgetrect.h"

WidgetRect::WidgetRect(): QObject()	
{

}

WidgetRect::~WidgetRect()
{

}

QRect WidgetRect::widgetGlobalRect(const QWidget* widget)
{
	QRect rect;
	rect = widget->rect();
	rect.setTopLeft(widget->mapToGlobal(rect.topLeft()));
	rect.setBottomRight(widget->mapToGlobal(rect.bottomRight()));
	return rect;
}
