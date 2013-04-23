#include "stdafx.h"
#include "BlurJudger.h"
#include "jhead/jpghead.h"

#define KENEL 3
#define ROISIZE 128

BlurJudger::BlurJudger()
	:m_max1(0.73f), m_max3(0.33f), m_threshold(30), m_force(0.8f)
{

}

BlurJudger::BlurJudger( int force )
{
	initParamWithForce(force);
}

BlurJudger::~BlurJudger()
{

}

void BlurJudger::initParamWithForce( int force )
{
	m_max1 = 0.30f;
	m_max3 = 0.70f; 
	m_threshold = 30;

	
	//m_force = force / 100.0f;
	m_force = 0.1;
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

// int BlurJudger::Judge( const QString imageName , bool* ret)
// {
// 	QString fullPath = FileUtils::getFullPath(imageName);
// 	Mat srcImg = resizeImamge(imread(fullPath.toStdString(), CV_LOAD_IMAGE_GRAYSCALE));
// 
// 	if(!srcImg.data)
// 	{
// 		*ret = false;
// 		return LoadImageError;
// 	}
// 
// 	int unSmooth, all;
// 	ImageDefinition def = calcImageDefinition(srcImg, &unSmooth, &all);
// 	if(def.m1 == 0.0f && def.m3 == 0.0f)
// 	{
// 		*ret = true;
// 		return NoJudge;
// 	}
// 
// 	float ratio =  unSmooth  / (float)all;
// 	int smooth = all - unSmooth;
// // 	int a = 0;
// // 	if(imageName.toUpper().endsWith("DSCN0528.JPG"))
// // 	{
// // 		a = 1;
// // 	}
// 	
// 	//if(a != 1){*ret = true; return 0; };
// 
// 	JpgHead picHeader;
// 	picHeader.processFile(fullPath);
// 	QVariantMap headerInfo = picHeader.getExif();
// 
// 	float expTime = headerInfo.value("ExposureTime", QVariant()).toFloat();
// 	int forceLength35 = headerInfo.value("FocalLength35mmEquiv", QVariant()).toInt();
// 
// 	float m_expTimeUpper = 1 / 9.0f;
// 	float m_expTimeLower = 1 / 500.0f;
// 	int flFactor  = 10;
// 
// 	if(expTime != 0.0f)
// 	{
// 		if( expTime >= m_expTimeUpper)
// 		{
// 			float ss = 1.0f / forceLength35;
// 			float fl = (forceLength35 == 0) ? 0.0f :  1.0f / forceLength35 * flFactor; 
// 			if(fl != 0.0f && expTime >= fl)
// 			{ //模糊的极少就算模糊;
// 				//*ret = def.m1 > 0.70f && def.m3 > 0.3f  ? true : false;
// 				*ret = ratio > 0.23f ? false : true;
// 				return NoError;
// 			}
// 			else
// 			{//少许模糊就算;
// 				*ret = ratio > 0.40f ? false : true;
// 				//*ret = def.m1 > 0.73f && def.m3 > 0.33f  ? true : false;
// 				return NoError;
// 			}
// 		}
// 		else if(expTime <= m_expTimeLower)
// 		{ // 全局模糊高强度;
// 			*ret = ratio > 0.69f ? false : true;
// 			return NoError;
// 		}
// 	}
// 
// // 	float upForce = min(10, 2 + m_force * m_force * 10.0f);
// // 	float lowForce = max(0.08, 0.3 - m_force * m_force);
// 
// 	float upForce = 0.89f;
// 	float lowForce = 0.5f;
// 	//正常检测;
// 	if(ratio <= upForce) 
// 	{
// 		*ret = true; // 太近似无法判断;
// 	}else if(ratio > upForce)
// 	{
// 		*ret = false;
// 	} 
// 
//    return NoError;
// }

Mat src;
int spatialRad=10,colorRad=40,maxPryLevel=3;

void meanshift_seg(int,void *)
 {
	 Mat dst(src.size(), src.type());
     pyrMeanShiftFiltering(src,dst,spatialRad,colorRad,maxPryLevel);
     RNG rng=theRNG();
//      Mat mask(dst.rows+2,dst.cols+2,CV_8UC1,Scalar::all(0));
//      for(int i=0;i<dst.rows;i++) 
//          for(int j=0;j<dst.cols;j++)
//              if(mask.at<uchar>(i+1,j+1)==0)
//              {
//                  Scalar newcolor(rng(256),rng(256),rng(256));
//                  floodFill(dst,mask,Point(i,j),newcolor,0,Scalar::all(1),Scalar::all(1));
//              }
	 imshow("dst",dst);
 }

int BlurJudger::Judge( const QString imageName , bool* ret)
{
	string fullPath = FileUtils::getFullPath(imageName).toStdString();
	src = resizeImamge(imread(fullPath));
	if(!src.data)
	{
		*ret = false;
		return LoadImageError;
	}


	namedWindow("src",WINDOW_AUTOSIZE);
	namedWindow("dst",WINDOW_AUTOSIZE);

	createTrackbar("spatialRad","dst",&spatialRad,80,meanshift_seg);
	createTrackbar("colorRad","dst",&colorRad,60,meanshift_seg);
	createTrackbar("maxPryLevel","dst",&maxPryLevel,5,meanshift_seg);

	Mat dst(src.size(), src.type());
	pyrMeanShiftFiltering(src,dst,spatialRad,colorRad,maxPryLevel);
	imshow("dst",dst);
	imshow("src",src);

	waitKey();
	return 0;
}
ImageDefinition BlurJudger::calcDefinition( Mat srcImg, Mat maskImg)
{
	float avgMax1(0), avgMax3(0);
	Mat gradImg = getGradientImage(srcImg);

	vector<Gradient> grads = calcMaxGradient(gradImg, maskImg);
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

ImageDefinition BlurJudger::calcImageDefinition( Mat srcImg, int* unSmoothCount = NULL, int* allCount = NULL)
{
	int unSmoothCnt(0), allCnt(0);
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

			imgDef.m1 >= m_max1 && imgDef.m3 >= m_max3 ?  NULL:  unSmoothCnt++;
			avgM1 += imgDef.m1;
			avgM3 += imgDef.m3;
			allCnt++;
		}
	}

	if(allCount) *allCount = allCnt;
	if(unSmoothCount) *unSmoothCount = unSmoothCnt;
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


//计算图像梯度;
Mat BlurJudger::getGradientImage( Mat graySrcImg )
{
	DebugAssert(graySrcImg.depth() == CV_8U);
	int imgType = CV_32FC1;
	Mat gradX, gradY;

	Sobel(graySrcImg, gradX, imgType, 1, 0, KENEL);
	Sobel(graySrcImg, gradY, imgType, 0, 1, KENEL);
//  	addWeighted(gradX, 0.5, gradY, 0.5, 0, gradX);
//  	return gradX;

	Mat gradImg = Mat::zeros(gradX.size(), imgType);
	magnitude(gradX, gradY, gradImg);
	return gradImg;
}


//计算图像边缘图像
Mat BlurJudger::getEdgeImage(Mat srcImg)
{
	Mat edgeImg, tmpImg;
	GaussianBlur(srcImg, tmpImg, Size(KENEL , KENEL), 0, 0);
	Canny(tmpImg, edgeImg, m_threshold, m_threshold * 5);
//  	erode(edgeImg, tmpImg, Mat(Size(3, 3), CV_8UC1));
//  	dilate(tmpImg, edgeImg, Mat(Size(3, 3), CV_8UC1));
	//imshow("sss", edgeImg);
	//waitKey(0);
	return edgeImg;
}

vector<Gradient> BlurJudger::calcMaxGradient( Mat srcImg, Mat maskImg)
{
	DebugAssert(srcImg.type() == CV_32F);
	vector<Gradient> vec;
	float grad0 ,grad45, grad90, grad135;
	int step = srcImg.step[0] / srcImg.step[1];

	//忽略了边缘的3个像素;
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
				clearNeighbor(maskImg, row, col, 7);
			}
			else
			{
				//降序排列;
				sort(vec.begin(), vec.end(), [](Gradient grad1, Gradient grad2){
					return grad1.value < grad2.value;});

					if(vec.at(0).value < valueMax)
					{
						vec[0] = Gradient(Point(row,col), valueMax, dir);
						clearNeighbor(maskImg, row, col, 7);
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


