///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: mfcFile.cpp
//
// Description:
//
// Created: 2015年05月26日 星期二 09时14分56秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////

#include "mfcFile.h"
#include <sys/types.h>
#include <sys/stat.h>

MfcFile::MfcFile()
{
}

MfcFile::~MfcFile()
{
}

unsigned int MfcFile::getFileSize(const std::string &fileName)
{
	struct stat statbuf;
	stat(fileName.c_str(), &statbuf);

	return statbuf.st_size;
}

bool MfcFile::reName(const std::string &oldPath, const std::string &newPath)
{
	if(rename(oldPath.c_str(), newPath.c_str()) != 0)
	{
		return false;
	}

	return true;
}

bool MfcFile::delFile(const std::string &filePath)
{
	if(remove(filePath.c_str()) != 0)
	{
		return false;
	}

	return true;
}

/* we create tmp file like this: */
/* /usr/tmp/demo.cpp --> /usr/tmp/.demo.cpp */
std::string MfcFile::createTmpFile(const std::string &srcFile)
{
	std::string::size_type found = srcFile.find_last_of("/");
	return srcFile.substr(0,found) + "/." + srcFile.substr(found+1);
}
