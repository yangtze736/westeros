///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName:   urlhandle.h
//
// Description:
//
// Created:    2015-06-30
// Revision:   Revision: 1.0
// Compiler:   g++
//
///////////////////////////////////////////////////////////

#ifndef _URLHANDLE_H_
#define _URLHANDLE_H_

#include <string.h>
#include <string>
#include <list>
#include <map>
#include <vector>

using namespace std;

// namespace URL is a frame about url operator
namespace URL
{
	// MD5 encryption algorithm
	bool md5Encrpt(string &md5OutPut, const string &originUrl);

	// function about encoding
	int UrlDecode(char *outUrl, const char *inUrl, int max_len);
	char CharToHex(char up, char low);
	char char_to_hex(char ch);
	//
	int UrlEncode(char *outUrl, const char *inUrl, int max_len);

	void string_gbk_to_utf8(string &out, const string &in);
	int gbk_to_utf8(char *out, const char *in, int max_len, int len);

	void string_utf8_to_gbk(string &out, const string &in);
	int utf8_to_gbk(char *out, const char *in, int max_len, int len);

	// base64
	int base64_encode(unsigned char *out, const unsigned char *in, int len, const char *encode_map);
	int base64_decode(unsigned char *out, const unsigned char *in, int len, const char *decode_index);
	int Base64Encode(unsigned char *out, const unsigned char *in, int len);
	int Base64Decode(unsigned char *out, const unsigned char *in, int len);

}
#endif
