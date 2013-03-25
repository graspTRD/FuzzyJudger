#include <QtGui>
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <Windows.h>

#include "fileutils.h"
#include "errorutils.h"

using namespace std;
using namespace cv;

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#if defined(_DEBUG)

#include <QDebug>
#define DebugAssert(condition) { \
	if(!(condition)) { \
	QString info = QString("File: %0\nLine: %1\n∂œ—‘ ß∞‹!").arg(__FILENAME__).arg(__LINE__); \
	Error::showError(info); \
	} \
}

#define BEGIN_EXEC \
	clock_t  _istart_time; \
	_istart_time = clock(); 

#define END_EXEC \
	clock_t  _iend_time; \
	_iend_time = clock(); \
	qDebug()<<__FUNCTION__<<" exec time is: "<<(double)(_iend_time - _istart_time) / CLOCKS_PER_SEC<<" sec!\n"; 

#else

#define DebugAssert(condition)
#define BEGIN_EXEC
#define END_EXEC

#endif