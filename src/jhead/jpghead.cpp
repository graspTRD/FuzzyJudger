#include "stdafx.h"
#include "jpghead.h"
#include <sys/stat.h>

JpgHead::JpgHead()
	:isProcessFile(false)
{
	
}

JpgHead::~JpgHead()
{

}

bool JpgHead::processFile(QString FileName)
{
	ReadMode_t ReadMode;
	ReadMode = READ_METADATA;
	ResetJpgfile();
	// Start with an empty image information structure.
	memset(&ImageInfo, 0, sizeof(ImageInfo));
	ImageInfo.FlashUsed = -1;
	ImageInfo.MeteringMode = -1;
	ImageInfo.Whitebalance = -1;

	// Store file date/time.
	struct stat st;
	if (stat(FileName.toAscii(), &st) >= 0){
		ImageInfo.FileDateTime = st.st_mtime;
		ImageInfo.FileSize = st.st_size;
	}else{
		return false;
	}

	strncpy(ImageInfo.FileName, FileName.toAscii(), PATH_MAX);
	ReadJpegFile(FileName.toAscii(), READ_METADATA);
	isProcessFile = true;
	return true;
}

QByteArray JpgHead::getThumbnail()
{
	if (!isProcessFile) return QByteArray();

	if (ImageInfo.ThumbnailOffset == 0 || ImageInfo.ThumbnailSize == 0){
		//Image contains no thumbnail;
		return QByteArray();
	}
	uchar * thumbnailPointer;
	Section_t * exifSection;
	exifSection = FindSection(M_EXIF);
	thumbnailPointer = exifSection->Data+ImageInfo.ThumbnailOffset+8;
	const char * datastart = (char *)thumbnailPointer;
	QByteArray data(datastart, ImageInfo.ThumbnailSize);
	return data;
}

QVariantMap JpgHead::getExif()
{
	if (!isProcessFile) return QVariantMap();
	QVariantMap map;
	map.insert("FileName", ImageInfo.FileName);
    //map.insert("FileDateTime", ImageInfo.FileDateTime);
    map.insert("FileSize", ImageInfo.FileSize);
	map.insert("CameraMake", ImageInfo.CameraMake);
	map.insert("CameraModel", ImageInfo.CameraModel);
	map.insert("DateTime", ImageInfo.DateTime);
	map.insert("Height", ImageInfo.Height);
	map.insert("Width", ImageInfo.Width);
	map.insert("Orientation", ImageInfo.Orientation);
	map.insert("IsColor", ImageInfo.IsColor);

	return map;
	//struct {
	//	// Info in the jfif header.
	//	// This info is not used much - jhead used to just replace it with default
	//	// values, and over 10 years, only two people pointed this out.
	//	char  Present;
	//	char  ResolutionUnits;
	//	short XDensity;
	//	short YDensity;
	//}JfifHeader;

	//int   Process;
	//int   FlashUsed;
	//float FocalLength;
	//float ExposureTime;
	//float ApertureFNumber;
	//float Distance;
	//float CCDWidth;
	//float ExposureBias;
	//float DigitalZoomRatio;
	//int   FocalLength35mmEquiv; // Exif 2.2 tag - usually not present.
	//int   Whitebalance;
	//int   MeteringMode;
	//int   ExposureProgram;
	//int   ExposureMode;
	//int   ISOequivalent;
	//int   LightSource;
	//int   DistanceRange;

	//float xResolution;
	//float yResolution;
	//int   ResolutionUnit;

	//char  Comments[MAX_COMMENT_SIZE];
	//int   CommentWidthchars; // If nonzero, widechar comment, indicates number of chars.

	//unsigned ThumbnailOffset;          // Exif offset to thumbnail
	//unsigned ThumbnailSize;            // Size of thumbnail.
	//unsigned LargestExifOffset;        // Last exif data referenced (to check if thumbnail is at end)

	//char  ThumbnailAtEnd;              // Exif header ends with the thumbnail
	//// (we can only modify the thumbnail if its at the end)
	//int   ThumbnailSizeOffset;

	//int  DateTimeOffsets[MAX_DATE_COPIES];
	//int  numDateTimeTags;

	//int GpsInfoPresent;
	//char GpsLat[31];
	//char GpsLong[31];
	//char GpsAlt[20];
}
