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

	private:
		bool rename_file(const char *src, const char *dst);
};

#endif
