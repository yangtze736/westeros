///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: mfcFile.h
//
// Description:
//
// Created: 2015年05月26日 星期二 09时10分09秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////
#ifndef __MFCFILE_H__
#define __MFCFILE_H__

#include <string>
#include <list>

typedef unsigned long int uoff_t;
#define READ_BUFSIZE 4096U

class MfcFile
{
	public:
		MfcFile();
		~MfcFile();

	public:
		static unsigned int getFileSize(const std::string &fileName);
		static bool reName(const std::string &oldPath, const std::string &newPath);
		static bool delFile(const std::string &filePath);
		static std::string createTmpFile(const std::string &srcFile);
		static int split(const std::string &srcFile, std::list<std::string> &fileList);
		static int splitFile(const char *filename, uoff_t s);
		static int mergeFile(const char *filename);

	private:
};

#endif
