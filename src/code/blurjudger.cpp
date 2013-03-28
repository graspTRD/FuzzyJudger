#include "stdafx.h"
#include "BlurJudger.h"

#define KENEL 3
#define ROISIZE 256

BlurJudger::BlurJudger()
	:m_max1(0.35f), m_max3(0.75f), m_threshold(25)
{

}

BlurJudger::~BlurJudger()
{

}

void BlurJudger::SetMax1Threshold( float m1 )
{
	DebugAssert(m1 > 0);
	m_max1 = m1;
}

void BlurJudger::SetMax3Threshold( float m3 )
{
	DebugAssert(m3 > 0);
	m_max3 = m3;
}

float BlurJudger::Max1Threshold() const
{
	return m_max1;
}

float BlurJudger::Max3Threshold() const
{
	return m_max3;
}

int BlurJudger::Judge( const QString imageName , bool* ret, ImageDefinition* outDef)
{
	string fullPath = FileUtils::getFullPath(imageName).toStdString();
	Mat srcImg = resizeImamge(imread(fullPath, CV_LOAD_IMAGE_GRAYSCALE));
	if(!srcImg.data)
	{
		*ret = false;
		return 1;
	}

	int smooth, all;
	ImageDefinition def = calcImageDefinition(srcImg, &smooth, &all);

	if(def.m1 == 0.0f && def.m3 == 0.0f)
	{
		*ret = false;
		return 1;
	}

	*ret = (def.m1 >= m_max1 && def.m3 >= m_max3);
	if(outDef)  *outDef = def;
	return 0;
}

ImageDefinition BlurJudger::calcDefinition( Mat srcImg, Mat maskImg)
{
	float avgMax1(0), avgMax3(0);
	Mat gradImg = getGradientImage(srcImg);

//	BEGIN_EXEC
	vector<Gradient> grads = calcMaxGradient(gradImg, maskImg);
//	END_EXEC
	if(grads.size() != 0 )
	{
		for(int i = 0; i < (int)grads.size(); i++)
		{
			vector<float> v = getNormalPoints (gradImg, grads.at(i));
			float total = accumulate(v.begin(), v.begin() + 6, 0.0f);
			avgMax1 += calcMax1(v) / total;
			avgMax3 += calcMax3(v) / total;
		}
		return std::move(ImageDefinition(avgMax1 / grads.size(), avgMax3 / grads.size()));
	}
	return std::move(ImageDefinition(0.0f, 0.0f));
}

ImageDefinition BlurJudger::calcImageDefinition( Mat srcImg, int* smoothCount = NULL, int* allCount = NULL)
{
	int smoothCnt(0), allCnt(0);
	float avgM1(0), avgM3(0);

	int imgRowStep = min(ROISIZE, srcImg.rows);
	int imgColStep = min(ROISIZE, srcImg.cols);

	int rowCount = (int)ceil(srcImg.rows / (imgRowStep * 1.0f));
	int colCount = (int)ceil( srcImg.cols / (imgColStep * 1.0f));
	int endWidth = srcImg.cols - imgColStep * (colCount - 1);
	int endHeight = srcImg.rows - imgRowStep * (rowCount - 1);

	Mat maskImg = getEdgeImage(srcImg);
	for(int i = 0; i < rowCount; i++)
	{
		int height = (i== rowCount - 1) ? endHeight: imgRowStep;
		for(int j = 0; j < colCount ; j++)
		{
			int width = (j == colCount - 1) ? endWidth: imgColStep; 
			Rect roi = Rect(j*imgColStep, i*imgRowStep, width, height);
			Mat roiImg(srcImg, roi);
			Mat roiMaskImg(maskImg, roi);

			ImageDefinition imgDef = calcDefinition(roiImg, roiMaskImg);
			if(imgDef.m1 == 0.0f && imgDef.m3 == 0.0f) continue;

			imgDef.m1 >= m_max1 && imgDef.m3 >= m_max3 ? smoothCnt++ : NULL ;
			avgM1 += imgDef.m1;
			avgM3 += imgDef.m3;
			allCnt++;
		}
	}

	if(allCount) *allCount = allCnt;
	if(smoothCount) *smoothCount = smoothCnt;
	if(allCnt == 0) return std::move(ImageDefinition(0.0f, 0.0f));

	return std::move(ImageDefinition(avgM1 / allCnt, avgM3 / allCnt));
}


vector<float> BlurJudger::getNormalPoints(Mat gradImg, Gradient grad )
{
	Direction norDir = getNormalDirection(grad);
	DebugAssert(norDir != Invalid);

	switch(norDir)
	{
	case Degree0: 
		return collectDegree0Points(gradImg, grad.pos, 7);
	case Degree45:
		return collectDegree45Points(gradImg, grad.pos, 7);
	case Degree90:
		return collectDegree90Points(gradImg, grad.pos, 7);
	case Degree135: 
		return collectDegree135Points(gradImg, grad.pos, 7);
	default:
		break;
	}

	return vector<float>();
}


//¼ÆËãÍ¼ÏñÌÝ¶È;
Mat BlurJudger::getGradientImage( Mat graySrcImg )
{
	DebugAssert(graySrcImg.depth() == CV_8U);
	int imgType = CV_32FC1;
	Mat gradX, gradY;

	Sobel(graySrcImg, gradX, imgType, 1, 0, KENEL);
	Sobel(graySrcImg, gradY, imgType, 0, 1, KENEL);
// 	addWeighted(gradX, 0.5, gradY, 0.5, 0, gradX);
// 	return gradX;

	Mat gradImg = Mat::zeros(gradX.size(), imgType);
	accumulateSquare(gradX, gradImg);
	accumulateSquare(gradY, gradImg);
	sqrt(gradImg, gradImg);
	return gradImg;
}


//¼ÆËãÍ¼Ïñ±ßÔµÍ¼Ïñ
Mat BlurJudger::getEdgeImage(Mat srcImg)
{
	Mat edgeImg, tmpImg;
	GaussianBlur(srcImg, tmpImg, Size(KENEL, KENEL), 0, 0);
	Canny(tmpImg, edgeImg, m_threshold, m_threshold * 3);
	return edgeImg;
// 	Mat element = getStructuringElement( MORPH_CROSS, Size( 3, 3) );
// 	dilate(edgeImg, tmpImg, element);
// 	erode( tmpImg, tmpImg, element);
// 	imshow("Edge Window", edgeImg);
// 	imshow("erode", tmpImg);
}

vector<Gradient> BlurJudger::calcMaxGradient( Mat srcImg, Mat maskImg)
{
	DebugAssert(srcImg.type() == CV_32F);
	vector<Gradient> vec;
	float grad0 ,grad45, grad90, grad135;
	int step = srcImg.step[0] / srcImg.step[1];

	//ºöÂÔÁË±ßÔµµÄ3¸öÏñËØ;
	for(int row = 3; row < maskImg.rows - 3; row++)
	{
		float* maskPtr = maskImg.ptr<float>(row);
		float* colPtr = srcImg.ptr<float>(row);
		
		for(int col = 3; col < maskImg.cols - 3; col++)
		{
			if(maskPtr[col] == 0) continue;
			float* pData = colPtr + col;

			grad0 = *(pData - 2) + *(pData - 1) + *pData +  *(pData + 1) + *(pData + 2);
			grad45 = *(pData + step*2 - 2) + *(pData + step - 1)+ *pData + *(pData - step + 1) + *(pData - step*2 + 2);
			grad90 = *(pData - step*2) + *(pData - step)+ *pData + *(pData + step) + *(pData + step*2);
			grad135 = *(pData - step*2 - 2) + *(pData - step - 1)+ *pData + *(pData + step - 1) + *(pData + step*2 - 2);

			float valueMax = max(max(grad0, grad45), max(grad90, grad135));
			Direction dir = getGradDirection(grad0, grad45, grad90, grad135);

			if(vec.size() < 3)
			{
				vec.push_back(Gradient(Point(row,col), valueMax, dir));
				//clearNeighbor(maskImg, row, col, 7);
			}
			else
			{
				//½µÐòÅÅÁÐ;
				sort(vec.begin(), vec.end(), [](Gradient grad1, Gradient grad2){
					return grad1.value < grad2.value;});

					if(vec.at(0).value < valueMax)
					{
						vec[0] = Gradient(Point(row,col), valueMax, dir);
						//clearNeighbor(maskImg, row, col, 7);
					}
			}
		}
	}
	return vec;
}

Direction BlurJudger::getNormalDirection( Gradient grad )
{
	int degree = (int)grad.dir + 90;
	degree = degree >= 180 ? degree - 180 : degree;

	if(degree != 0 && degree != 45 && degree != 90 && degree != 135)
	{
		degree = -1;
	}

	return (Direction)degree;
}

float BlurJudger::getImageVauleAt( Mat srcImg, int row, int col )
{
	DebugAssert(srcImg.channels() == 1);
	DebugAssert(row < srcImg.rows && row >= 0);
	DebugAssert(col < srcImg.cols && col >= 0);

	float* data = srcImg.ptr<float>(row);
	return data[col];
} 

vector<float> BlurJudger::collectDegree0Points(Mat gradImg, const Point& pt, int count )
{
	int n = count / 2 ;

	DebugAssert(count % 2!=0);
	DebugAssert(pt.x >= n && pt.x < gradImg.rows - n);

	vector<float> grads;
	float* data = gradImg.ptr<float>(pt.x) + pt.y;
	for(int i = -n; i <= n; i++)
	{
		grads.push_back(*(data + i));
	}
// 	cout<<"0 degree-2: ";
// 	std::for_each(grads.begin(), grads.end(), [](float d){cout<<d<<" ";});
// 	cout<<endl;
	return grads;
}

vector<float> BlurJudger::collectDegree45Points( Mat gradImg, const Point& pt, int count )
{
	int n = count / 2;
	
	DebugAssert(count % 2!=0);
	DebugAssert(pt.x >= n && pt.x < gradImg.rows - n);
	DebugAssert(pt.y >= n && pt.y < gradImg.cols - n);	

	int i, j;
	vector<float> grads;
	int step = gradImg.step[0] / gradImg.step[1];

	float* data = gradImg.ptr<float>(pt.x) + pt.y;
	for(i = -n, j = n; i <= n; i++, j--)
	{
		grads.push_back(*(data + j*step + i));
	}
// 	cout<<"45 degree-2: ";
// 	std::for_each(grads.begin(), grads.end(), [](float d){cout<<d<<" ";});
// 	cout<<endl;
	return grads;
}

vector<float> BlurJudger::collectDegree90Points( Mat gradImg, const Point& pt, int count )
{
	int n = count / 2;

	DebugAssert(count % 2!=0);
	DebugAssert(pt.y >= n && pt.y < gradImg.cols - n);

	vector<float> grads;
	float* data = gradImg.ptr<float>(pt.x) + pt.y;
	int step = gradImg.step[0] / gradImg.step[1];

	for(int i = -n; i <= n; i++)
	{
		grads.push_back(*(data + i*step));
	}
// 	cout<<"90 degree-2: ";
// 	std::for_each(grads.begin(), grads.end(), [](float d){cout<<d<<" ";});
// 	cout<<endl;
	return grads;
}

vector<float> BlurJudger::collectDegree135Points( Mat gradImg, const Point& pt, int count )
{
	int n = count / 2;
	DebugAssert(count % 2!=0);
	DebugAssert(pt.x >= n && pt.x < gradImg.rows - n);
	DebugAssert(pt.y >= n && pt.y < gradImg.cols - n);
	DebugAssert(gradImg.type() == CV_32F);

	vector<float> grads;
	float* data = gradImg.ptr<float>(pt.x) + pt.y;
	
	int step = gradImg.step[0] / gradImg.step[1];

	for(int i = -n; i <= n; i++)
	{
		grads.push_back(*(data + i*step + i));
	}
// 	cout<<"135 degree-2: ";
// 	std::for_each(grads.begin(), grads.end(), [](float d){cout<<d<<" ";});
// 	cout<<endl;
	return grads;
}

float BlurJudger::calcSum( const vector<float> vec )
{
	return std::accumulate(vec.begin(), vec.end(), 0.0f);
}


Direction BlurJudger::getGradDirection( float gard0, float grad45, float grad90, float grad135 )
{
	float maxGrad = max(max(gard0, grad45), max(grad90, grad135));

	if(maxGrad == gard0)
	{
		return Degree0;
	}
	else if(maxGrad == grad45)
	{
		return Degree45;
	}
	else if(maxGrad == grad90)
	{
		return Degree90;
	}
	else if(maxGrad == grad135)
	{
		return Degree135;
	}

	return Invalid;
}

float BlurJudger::calcMax1( const vector<float> vec )
{
	DebugAssert(vec.size() > 5);
	return max(max(vec.at(1), vec.at(2)), max(vec.at(3), vec.at(4)));
}

float BlurJudger::calcMax3( const vector<float> vec )
{
	DebugAssert(vec.size() > 5);

	float d1 = vec.at(1) + vec.at(2) + vec.at(3);
	float d2 = vec.at(2) + vec.at(3) + vec.at(4);

	return max(d1, d2);
}

Mat BlurJudger::resizeImamge( Mat srcImg )
{
	int orgW = srcImg.cols;
	int orgH = srcImg.rows;
	int cpsW = 1200;
	int cpsH = 900;

	if(orgW <= cpsW && orgH <= cpsH)
	{
		return srcImg;
	}

	float rotio = min((float)cpsW / orgW, (float)cpsH / orgH);
	int dstW = ceil(orgW * rotio);
	int dstH = ceil(orgH * rotio);

	Mat newImg;
	resize(srcImg, newImg, Size(dstW, dstH));
	return newImg;
}

void BlurJudger::clearNeighbor( Mat srcImg, int row, int col, int nCount )
{
	DebugAssert(srcImg.type() == CV_8U);
	int step = srcImg.step[0] / srcImg.step[1];
	int allRow = srcImg.rows;
	int allCol = srcImg.cols;
	int currentRow = row;

	uchar* pMask = srcImg.ptr<uchar>(row);

	for(int i = 0; i < nCount && currentRow < allRow; i++, currentRow += i)
	{
		pMask += i*step;
		for(int j = 0; j < nCount; j++)
		{	
			int currCol = col + j;	
			if((j == 0 && i == 0 ) ||
				currCol >= allCol ) 
				continue;
			uchar* pData = pMask + currCol;
			*pData = 0;
		}
	}
}

