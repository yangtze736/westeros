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

int MfcFile::split(const std::string &srcFile, std::list<std::string> &fileList)
{
	char dest_file[256] = {0};

	unsigned long file_size = 100*1024*1024;//100M
	unsigned long FILE_SIZE = file_size;

	snprintf(dest_file,256,"%s.dst",srcFile.c_str());


	FILE* src_file = fopen(srcFile.c_str(),"rb");
	if(src_file == NULL)
	{
		printf("open %s file error!\n",srcFile.c_str());
		return -1;
	}

	char* data_block = new char[file_size];
	FILE* dst_file = NULL;
	char split_file[256];
	unsigned int count = 0;

	//write the same size file
	while((file_size = fread(data_block,1,FILE_SIZE,src_file)) == FILE_SIZE)
	{
		snprintf(split_file,256,"%s%04d",dest_file,count++);
		fileList.push_back(std::string(split_file));
		dst_file = fopen(split_file,"wb");
		if(dst_file == NULL)
		{
			printf("open %s error!\n",split_file);
			return -1;
		}
		fwrite(data_block,1,file_size,dst_file);
		fclose(dst_file);
	}

	//write the last block data 
	snprintf(split_file,256,"%s%04d",dest_file,count);
	fileList.push_back(std::string(split_file));
	dst_file = fopen(split_file,"wb");
	if(dst_file == NULL)
	{
		printf("open %s error!\n",split_file);
		return -1;
	}
	fwrite(data_block,1,file_size,dst_file);
	fclose(dst_file);

	delete[] data_block;
	//close the source file
	fclose(src_file);

	return 0;
}

bool MfcFile::rename_file(const char *src, const char *dst)
{
	// read file to buffer
	FILE *fr = fopen(src, "rb");
	fseek(fr, 0, SEEK_END);
	long lsize = ftell(fr);
	rewind(fr);

	char *buffer = (char*)malloc(sizeof(char)*lsize);
	if(buffer == NULL)
		return -1;

	size_t result = fread(buffer, sizeof(char), lsize, fr);
	if(result != lsize)
		return -1;

	// buffer write to file
	FILE *fw = fopen(dst, "wb");
	fwrite(buffer, sizeof(char), lsize, fw);

	//
	free(buffer);
	fclose(fr);fclose(fw);

	return true;
}
