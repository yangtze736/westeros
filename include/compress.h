///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: compress.h
//
// Description:
//
// Created: 2015年07月14日 星期二 09时28分08秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////

#ifndef __COMPRESS_H__
#define __COMPRESS_H__

#include "snappy.h"

#define CRASH_UNLESS(condition) \
	(condition) ? (void)0 : \
	snappy::LogMessageVoidify() & snappy::LogMessageCrash()

#define CHECK_OK(cond) (cond).CheckSuccess()
#define CHECK_LE(a, b) CRASH_UNLESS((a) <= (b))
#define CHECK_EQ(a, b) CRASH_UNLESS((a) == (b))
#define CHECK(cond) CRASH_UNLESS(cond)

inline char *string_as_array(string *str){
	return str->empty() ? NULL : &*str->begin();
}

namespace snappy{
	class LogMessage {
		public:
			LogMessage() { }
			~LogMessage() { cerr << endl; }
			LogMessage& operator<<(const std::string& msg){
				cerr << msg;
				return *this;
			}
			LogMessage& operator<<(int x) {
				cerr << x;
				return *this;
			}
	};
	class LogMessageCrash : public LogMessage {
		public:
			LogMessageCrash(){ }
			~LogMessageCrash(){
				cerr << endl;
				abort();
			}
	};
	class LogMessageVoidify {
		public:
			LogMessageVoidify() { }
			void operator&(const LogMessage&) { }
	};
}

namespace file{

	class DummyStatus{
		public:
			void CheckSuccess(){}
	};

	DummyStatus GetContents(const string& filename, string *data)
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
			data->append(string(buf, ret));
		}

		fclose(fp);

		return DummyStatus();
	}

	DummyStatus SetContents(const string &filename, const string &str)
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

} // namespace file
#if 0
static bool Compress(const char *input, size_t input_size, string *compressed)
{
	size_t destlen;
	snappy::RawCompress(input,input_size,string_as_array(compressed),&destlen);
	CHECK_LE(destlen,snappy::MaxCompressedLength(input_size));
	
	return true;
}
static bool CheckUncompressedLength(const string& compressed, size_t *ulength)
{
	const bool result1 = snappy::GetUncompressedLength(compressed.data(),compressed.size(),ulength);
	snappy::ByteArraySource source(compressed.data(),compressed.size());
	unsigned int length;
	const bool result2 = snappy::GetUncompressedLength(&source, &length);
	CHECK_EQ(result1, result2);

	return result1;
}
#endif

static void CompressFile(const char *fname)
{
	string fullinput;
	CHECK_OK(file::GetContents(fname, &fullinput));

	string compressed;
	snappy::Compress(fullinput.data(), fullinput.size(), &compressed);

	CHECK_OK(file::SetContents(string(fname).append(".comp"),compressed));
}

static void UncompressFile(const char *fname)
{
	string fullinput;
	CHECK_OK(file::GetContents(fname, &fullinput));

	//size_t uncompLength;
	//CHECK(CheckUncompressedLength(fullinput, &uncompLength));

	string uncompressed;
	//uncompressed.resize(uncompLength);
	CHECK(snappy::Uncompress(fullinput.data(), fullinput.size(), &uncompressed));

	CHECK_OK(file::SetContents(string(fname).append(".uncomp"),uncompressed));
}

#endif //__COMPRESS_H__
