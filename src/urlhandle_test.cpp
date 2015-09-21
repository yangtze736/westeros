///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: src/urlhandle_test.cpp
//
// Description:
//
// Created: 2015年09月18日 星期五 14时20分10秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////

#include "testharness.h"
#include "baseFunc.h"

namespace middleware {

class UrlhandleTest { };

TEST(UrlhandleTest, Simple)
{
	// test base64Encode base64Decode
	std::string original0 = "https://1.2.3.4:443/测试 BASE64";
	fprintf(stdout, "original0 [%s]\n", original0.c_str());

	std::string original1 = base64Encode(original0);
	fprintf(stdout, "original1 [%s]\n", original1.c_str());

	std::string original2 = base64Decode(original1);
	fprintf(stdout, "original2 [%s]\n\n", original2.c_str());

	// test encode && decode
	std::string str1 = "https://1.2.3.4:443/测试 ENCODE & DECODE ?op=CREATE";
	fprintf(stdout, "str1 [%s]\n",str1.c_str());std::string str2 = urlEncode(str1);
	fprintf(stdout, "str2 [%s]\n",str2.c_str());std::string str3 = urlDecode(str2);
	fprintf(stdout, "str3 [%s]\n",str3.c_str());
}

} // namespace middleware

int
main(int argc, char *argv[]){
	return middleware::test::RunAllTests();
}

