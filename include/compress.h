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
#include <iostream>

#define CRASH_UNLESS(condition) \
	(condition) ? (void)0 : \
	snappy::LogMessageVoidify() & snappy::LogMessageCrash()

#define CHECK_OK(cond) (cond).CheckSuccess()
#define CHECK_LE(a, b) CRASH_UNLESS((a) <= (b))
#define CHECK_EQ(a, b) CRASH_UNLESS((a) == (b))
#define CHECK(cond) CRASH_UNLESS(cond)

inline char *string_as_array(std::string *str){
	return str->empty() ? NULL : &*str->begin();
}

namespace snappy{
	class LogMessage {
		public:
			LogMessage() { }
			~LogMessage() { std::cerr << std::endl; }
			LogMessage& operator<<(const std::string &msg){
				std::cerr << msg;
				return *this;
			}
			LogMessage& operator<<(int x) {
				std::cerr << x;
				return *this;
			}
	};
	class LogMessageCrash : public LogMessage {
		public:
			LogMessageCrash(){ }
			~LogMessageCrash(){
				std::cerr << std::endl;
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

	DummyStatus GetContents(const std::string &filename, std::string *data);
	DummyStatus SetContents(const std::string &filename, const std::string &str);

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
	std::string fullinput;
	CHECK_OK(file::GetContents(fname, &fullinput));

	std::string compressed;
	snappy::Compress(fullinput.data(), fullinput.size(), &compressed);

	CHECK_OK(file::SetContents(std::string(fname).append(".comp"),compressed));
}

static void UncompressFile(const char *fname)
{
	std::string fullinput;
	CHECK_OK(file::GetContents(fname, &fullinput));

	//size_t uncompLength;
	//CHECK(CheckUncompressedLength(fullinput, &uncompLength));

	std::string uncompressed;
	//uncompressed.resize(uncompLength);
	CHECK(snappy::Uncompress(fullinput.data(), fullinput.size(), &uncompressed));

	CHECK_OK(file::SetContents(std::string(fname).append(".uncomp"),uncompressed));
}

#endif //__COMPRESS_H__
