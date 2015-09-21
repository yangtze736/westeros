///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: src/snappy_test.cpp
//
// Description:
//
// Created: 2015年09月18日 星期五 14时35分18秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////

#include "testharness.h"
#include "snappy.h"
#include "compress.h"

namespace middleware {

class SnappyTest { };

TEST(SnappyTest, Simple)
{
	// test compress && decompress
	std::string ss;for(int i=0;i<100;i++)ss+="test-";ss+="finish!";
	std::string compress;
	int compress_len = snappy::Compress(ss.c_str(),ss.length(),&compress);
	fprintf(stdout, "Input.len: %d\nInput:\n[%s]\n\n", \
			(int)ss.length(),ss.c_str());
	fprintf(stdout, "Compress.len: %d\nCompress:\n[%s]\n\n", \
			compress_len,compress.c_str());

	std::string sRecovery;
	bool b = snappy::Uncompress(compress.c_str(),compress_len,&sRecovery);
	fprintf(stdout, "Recovery.len: %d\nRecovery:\n[%s]\n", \
			(int)sRecovery.length(),sRecovery.c_str());

	// test compress file && decompress file
	//CompressFile("put your compress file here!");
	//UncompressFile("put your uncompress file here!");
}

} // namespace middleware

int
main(int argc, char *argv[]){
	return middleware::test::RunAllTests();
}

