#include "stdafx.h"
#include "fileutils.h"
#include "errorutils.h"
#include "stringutils.h"
#include <string>
#include <QFileInfo>
#include <QFileInfoList>
#include "jhead/jpghead.h"
#pragma comment (lib, "Version.lib")

FileUtils::FileUtils()
{

}


FileUtils::~FileUtils()
{

}


QString FileUtils::getAppInstancePath()
{
	QString exeFileName = QApplication::applicationFilePath();
	QFileInfo file(exeFileName);
	return file.canonicalPath();
}

QString FileUtils::getFullPath(const QString& fileName)
{
	if(QDir::isAbsolutePath(fileName))
	{
		return fileName;
	}

	return  getAppInstancePath() + "/" + fileName;
}

bool FileUtils::existsFullPath(const QString& fileName)
{
	QFileInfo file(getFullPath(fileName));
	return file.exists();
}

bool FileUtils::exists(const QString& fileName)
{
	QString newFileName = getFullPath(fileName);
	return existsFullPath(newFileName);
}

QString FileUtils::readAllText(const QString &fileName)
{
	QString fullPath = getFullPath(fileName);
	if(existsFullPath(fullPath)) return readAllTextFullPath(fullPath);
	return "";
}

bool FileUtils::writeText(const QString& fileName, QString& content)
{
	QFile file(FileUtils::getFullPath(fileName));
	if(!file.open(QIODevice::ReadWrite))
	{
		return false;
	}
	QTextStream out(&file);
	out<<content;
	file.close();
	return true;
}

QByteArray FileUtils::readLocalFile(const QString& path)
{
	QString fileName = getFullPath(path);
	QFile file(fileName);

	if(!file.open(QIODevice::ReadOnly))
	{
		Error::showError("打开文件失败!");
		return 0;
	}

	return file.readAll();
}

QString FileUtils::readAllTextFullPath(const QString& fileName)
{
	QFile file(fileName);

	if(!file.open(QIODevice::ReadOnly))
	{
		Error::showError("文件打开失败!");
		return "";
	}
	QTextStream os(&file);
	QString content = os.readAll();
	file.close();
	return content;
}

bool FileUtils::makeDir(const QString& strDir)
{
	if(strDir.isEmpty())  return false; 
	QString fullPath = getFullPath(strDir);
	QDir newDir;
	if(newDir.exists(fullPath)) return true;

	return newDir.mkdir(fullPath);
}

bool FileUtils::makePath(const QString& strPath)
{
	QString fullPath = getFullPath(strPath);
	QFileInfo pathInfo(fullPath);
	//if (!pathInfo.isDir()) return false;
	if(pathInfo.exists()) return true;

	QDir newPath;
	return newPath.mkpath(fullPath);
}

bool FileUtils::saveFile(const QByteArray* content, const QString& fileName, bool bCover)
{
	if(!content || fileName.isEmpty()) return false;
	//if(makePath(dir)) return false;
	int pos = fileName.lastIndexOf('/');
	if(pos>0) makePath(fileName.left(pos));

	QString fullPath  = getFullPath(fileName);
	QFile file(fullPath);
	
	int nByte = 0;
	if(!bCover)
	{
		if(file.exists()) return false;
	}

	if(!file.open(QIODevice::WriteOnly))
	{
		Error::showError("创建文件失败");
		return false;
	}
	nByte = file.write(content->data(), content->length());
	file.close();
	return nByte<0 ? false: true;
}


QStringList FileUtils::getFileList(const QString& dir)
{
	QDir dirInfo(dir);
	return dirInfo.entryList(QDir::Files);
}

bool FileUtils::copyFile(const QString& srcFile, const QString& destFile)
{
	QString srcFullPath = getFullPath(srcFile);
	QString dstFullPath = getFullPath(destFile);
	if(!existsFullPath(srcFullPath)) return false;
	if(!srcFullPath.compare(dstFullPath, Qt::CaseInsensitive)) return true;
	QFileInfo fileInfo(dstFullPath);
	if(!fileInfo.exists())	
	{
		makePath(fileInfo.path());
	}
	else if(existsFullPath(dstFullPath)) 
	{
		QFile::remove(dstFullPath);
	}
	return QFile::copy(srcFullPath, dstFullPath);
}

/*
	删除非空文件夹
*/
void FileUtils::rmPath(const QString& strDir)
{
	QString dstDir = getFullPath(strDir);
	QFileInfo file(dstDir);
	delDir(file);
}

void FileUtils::rmPath(const QString& strDir, const QString& exceptDir)
{
	QString dstDir = getFullPath(strDir);
	QFileInfo file(dstDir);
	delDir(file, getFullPath(exceptDir));
}

void FileUtils::delDir(QFileInfo rmFile, const QString& exceptDir)
{
	if(rmFile.isDir() && rmFile.path() == exceptDir) return;
	return delDir(rmFile);
}

void FileUtils::delDir(QFileInfo rmFile)
{
	if(rmFile.isDir())
	{
		int childCount =0;
		QString dir = rmFile.filePath();
		QDir thisDir(dir);
		childCount = thisDir.entryInfoList().count();
		QFileInfoList newFileList = thisDir.entryInfoList();
		if(childCount>2){
			for(int i=0;i<childCount;i++){
				if(newFileList.at(i).fileName() == "." || newFileList.at(i).fileName() == ".."){
					continue;
				}
				delDir(newFileList.at(i));
			}
		}
		rmFile.absoluteDir().rmpath(rmFile.fileName());
	}
	else if(rmFile.isFile())
	{
		rmFile.absoluteDir().remove(rmFile.fileName());
	}
}


int FileUtils::fileCount(const QString& filePath)
{
	int fCount = 0;
	fileCounter(filePath, &fCount);
	return fCount;
}

void FileUtils::fileCounter(const QString& filePath, int* pCounter)
{
	QFileInfo curFile(filePath);
	if(!curFile.exists()) return;

	if (curFile.isDir())
	{
		QString dir = curFile.filePath();
		QDir curDir(dir);
		int childCount = curDir.entryInfoList().count();
		QFileInfoList newFileList = curDir.entryInfoList();
		if(childCount > 2)
		{
			for(int i = 0;i< childCount; i++)
			{
				if(newFileList.at(i).fileName() == "." || newFileList.at(i).fileName() == "..")
					continue;
				fileCounter(newFileList.at(i).filePath(), pCounter);
			}
		}
	}
	else
	{
		if(pCounter) *pCounter += 1;
	}
}

bool FileUtils::isAbsolute(const QString& filePath) 
{
	QFileInfo fInfo(filePath);
	return fInfo.isAbsolute();
}

bool FileUtils::rmFile(const QString& fileName)
{
	if(!FileUtils::exists(fileName)) return true;
	return QFile::remove(FileUtils::getFullPath(fileName));
}

//************************************************************************
// Method:    renameFile	重命名文件
// Access:    public static 
// Returns:   bool
// Parameter: fileName
// Parameter: newName
//************************************************************************
bool FileUtils::renameFile( const QString& fileName, const QString& newName )
{
	if(!FileUtils::exists(fileName)) return false;
	QString srcName = getFullPath(fileName);
	if(FileUtils::isAbsolute(newName))
	{
	  return QFile::rename(srcName, newName);
	}
	QString destName = StringUtils::lastLeft(fileName, '/', true) + newName;
	return QFile::rename(srcName, destName);
}

//***********************************************************************
// Method:    createFile	创建文件
// Access:    public static 
// Returns:   bool
// Parameter: fileName		创建的文件路径
// Parameter: initSize		文件的初始化大小
// Parameter: bForce		是否强制创建不存在的路径,该值为true时,
//							如果路径不存在,创建路径;
//***********************************************************************
bool FileUtils::createFile( const QString& fileName, int initSize /*= 0*/, bool bForce /*= false*/ )
{
	QString fileFullPath = getFullPath(fileName);
	QFileInfo fileInfo(fileFullPath);
	if(fileFullPath.isEmpty() || fileInfo.isDir())
		return false;

	QFile file(fileFullPath);
	if(bForce && !QFileInfo(fileInfo.path()).exists())
		makePath(fileInfo.path());

	bool bSuc = true;
	if(!file.open(QIODevice::ReadWrite)) return false;
	if(initSize > 0) bSuc = file.resize(initSize);
	file.close();

	return bSuc;
}

//*******************************************************************************/

QString FileUtils::getFileDate(const QString &fileName)
{
	QFileInfo *info = new QFileInfo(fileName);
	QString date = info->created().toString(Qt::LocalDate);//.toString("yyyy-MM-dd");DateFormat::
	delete(info);
	return date;
}

QString FileUtils::getImageDateInfo( const QString& imgName )
{
	QString dateString = "";
	if (imgName.endsWith(".jpg", Qt::CaseInsensitive))
	{
		JpgHead jh;
		jh.processFile(imgName);
		QVariantMap imageInfo = jh.getExif();
		qDebug()<<imageInfo;
		if (!imageInfo.isEmpty())
		{
			dateString = imageInfo.value("DateTime").toString();
			return dateString;
		}
	}
	dateString = getFileDate(imgName);
	return dateString;
}

QByteArray FileUtils::getThumbnail(const QString &fileName)
{
	//读取jpg格式的缩略图
	QByteArray data;
	if (fileName.endsWith(".jpg",Qt::CaseInsensitive))
	{
		JpgHead jh;
		jh.processFile(fileName);
		data = jh.getThumbnail();
		if (data.size() > 0) return data;
	}
	return readImage(fileName,163).toByteArray();
}

QVariant FileUtils::readImage(const QString &fileName,const int &length,const int &quality)
{
	QByteArray data;
	QImageReader image_reader(fileName);
	int image_width = image_reader.size().width();
	int image_height = image_reader.size().height();
	int w;
	int h;

	if (length != 0)
	{
		if (image_width > image_height) {
			if (image_width < length) w = image_width;
			else w = length;
			image_height = static_cast<double>(w) / image_width * image_height;
			image_width = w;
		} else if (image_width < image_height) {
			if(image_height < length) h = image_height;
			else h = length;
			image_width = static_cast<double>(h) / image_height * image_width;
			image_height = h;
		} else {
			if(image_width > length)
			{
				image_width = length;
				image_height = length;
			}
		}
	}
	if (image_width == 0 )
	{
		image_width = image_reader.size().width();
	}
	if (image_height == 0 )
	{
		image_height = image_reader.size().height();
	}
	image_reader.setScaledSize(QSize(image_width, image_height));
	QImage image = image_reader.read();
	QBuffer buffer(&data);
	buffer.open(QIODevice::WriteOnly);
	if (fileName.endsWith(".png",Qt::CaseInsensitive)) image.save(&buffer, "png",quality);
	else image.save(&buffer, "jpeg",quality);

	//QImageReader无法获取图片信息时，用QFile获取
	if (!data.size())
	{
		QString url = "";// = getFullPath(fileName);

		if(QDir::isAbsolutePath(fileName)) url = fileName;	
		else{
			QString exeFileName = QApplication::applicationFilePath();
			QFileInfo file(exeFileName);
			url = file.canonicalPath(); // 去掉..后的，如E:/trd/product/Rokh/bin/
			url += "/" + fileName;	
		}


		QFile file(url);
		if (!file.open(QIODevice::ReadOnly))
		{
			return QVariant();
		}
		QByteArray arraydata = file.readAll();
		file.close();

		image.loadFromData(arraydata);
		image_reader.read(&image);
		image_width = image.size().width();
		image_height = image.size().height();
		if (length != 0)
		{
			if (image_width > image_height) {
				if (image_width < length) w = image_width;
				else w = length;
				image_height = static_cast<double>(w) / image_width * image_height;
				image_width = w;
			} else if (image_width < image_height) {
				if(image_height < length) h = image_height;
				else h = length;
				image_width = static_cast<double>(length) / image_height * image_width;
				image_height = h;
			} else {
				if(image_width > length)
				{
					image_width = length;
					image_height = length;
				}
			}
		}
		if (image_width == 0 )
		{
			image_width = image.size().width();
		}
		if (image_height == 0 )
		{
			image_height = image.size().height();
		}
		image = image.scaled(QSize(image_width, image_height));
		if (fileName.endsWith(".png",Qt::CaseInsensitive)) image.save(&buffer, "png",quality);
		else image.save(&buffer, "jpeg",quality);
	}
	buffer.close();
	return data;
}

