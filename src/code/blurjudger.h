#ifndef BLURJUDGER_H
#define BLURJUDGER_H
#include "stdafx.h"
#include <numeric>

enum Direction
{
	Invalid = -1,
	Degree0 = 0,
	Degree45 = 45,
	Degree90 = 90,
	Degree135 = 135
};

class Gradient
{
public:
	Gradient(Point pos, float value, Direction dir)
	{
		this->pos = pos;
		this->value = value;
		this->dir = dir;
	}
	Point pos;
	float value;
	Direction dir;
};

class ImageDefinition
{

public:
	ImageDefinition()
		:m1(0.0f), m3(0.0f)
	{ }
	ImageDefinition(float m1, float m3)
	{
		this->m1 = m1;
		this->m3 = m3;
	}
	float m1;
	float m3;
};

class BlurJudger
{
public:
	BlurJudger();
	~BlurJudger();

	int Judge(const QString imageName, bool* ret, ImageDefinition* outDef = NULL);

	void SetMax1Threshold(float m1);
	void SetMax3Threshold(float m2);

	float Max1Threshold() const;
	float Max3Threshold() const;

private:
	ImageDefinition calcDefinition(Mat srcImg, Mat maskImg);
	ImageDefinition calcImageDefinition(Mat srcImg, int* smoothCount, int* allCount);

	float calcMax1(const vector<float> vec);
	float calcMax3(const vector<float> vec);
	float calcSum(const vector<float> vec);
	float getImageVauleAt(Mat srcImg, int row, int col);

	Direction getNormalDirection(Gradient grad);
	Direction getGradDirection(float gard0, float grad45, float grad90, float grad135);

	Mat getEdgeImage(Mat srcImg);
	Mat getGradientImage(Mat graySrcImg);

	vector<Gradient> calcMaxGradient(Mat srcImg, Mat maskImg);
	vector<float> getNormalPoints(Mat gradImg, Gradient grad);

	vector<float> collectDegree0Points(Mat gradImg, const Point& pt, int count);
	vector<float> collectDegree45Points(Mat gradImg, const Point& pt, int count);
	vector<float> collectDegree90Points(Mat gradImg, const Point& pt, int count);
	vector<float> collectDegree135Points(Mat gradImg, const Point& pt, int count);

private:
	float m_max1;
	float m_max3;
	float m_threshold;
};


#endif //BLURJUDGER_H




