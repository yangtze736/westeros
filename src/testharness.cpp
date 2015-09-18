///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: src/testharness.cpp
//
// Description:
//
// Created: 2015年09月18日 星期五 14时05分52秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////

#include "testharness.h"

#include <string>
#include <vector>

namespace middleware {
namespace test {

namespace {
struct Test {
	const char *base;
	const char *name;
	void (*func)();
};
std::vector<Test> *tests;
}

bool RegisterTest(const char* base, const char* name, void (*func)())
{
	if(tests == NULL){
		tests = new std::vector<Test>;
	}

	Test t;
	t.base = base;
	t.name = name;
	t.func = func;
	tests->push_back(t);

	return true;
}

int RunAllTests()
{
	int num = 0;
	if(tests != NULL){
		for(size_t i=0; i < tests->size(); i++){
			const Test& t = (*tests)[i];

			fprintf(stdout, "==== Test %s.%s\n", t.base, t.name);
			(*t.func)();
			num++;
		}
	}
	fprintf(stdout, "==== PASSED %d tests\n", num);

	return 0;
}

} // namespace test
} // namespace middleware

