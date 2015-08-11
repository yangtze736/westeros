///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: compress.cpp
//
// Description:
//
// Created: 2015年07月20日 星期一 14时23分15秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////

#include "compress.h"

namespace file
{
	DummyStatus GetContents(const std::string &filename, std::string *data)
	{
		FILE *fp = fopen(filename.c_str(), "rb");
		if(fp == NULL){
			perror(filename.c_str());
			exit(1);
		}

		data->clear();
		while(!feof(fp)){
			char buf[4096];
			size_t ret = fread(buf, 1, 4096, fp);
			if(ret == 0 && ferror(fp)){
				perror("fread");
				exit(1);
			}
			data->append(std::string(buf, ret));
		}

		fclose(fp);

		return DummyStatus();
	}

	DummyStatus SetContents(const std::string &filename, const std::string &str)
	{
		FILE *fp= fopen(filename.c_str(), "wb");
		if(fp == NULL){
			perror(filename.c_str());
			exit(1);
		}

		int ret = fwrite(str.data(), str.size(), 1, fp);
		if(ret != 1){
			perror("fwrite");
			exit(1);
		}

		fclose(fp);

		return DummyStatus();
	}
}
