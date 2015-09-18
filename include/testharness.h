///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: include/testharness.h
//
// Description:
//
// Created: 2015年09月18日 星期五 13时53分42秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////

#ifndef __TESTHARNESS_H__
#define __TESTHARNESS_H__

namespace middleware {
namespace test {

extern int RunAllTests();

#define TCONCAT(a,b) TCONCAT1(a,b)
#define TCONCAT1(a,b) a##b

#define TEST(base,name)                                            \
class TCONCAT(_Test_,name) : public base {                         \
	public:                                                        \
		void _Run();                                               \
		static void _RunIt() {                                     \
			TCONCAT(_Test_,name) t;                                \
			t._Run();                                              \
		}                                                          \
};                                                                 \
bool TCONCAT(_Test_ignored_,name) =                                \
	::middleware::test::RegisterTest(#base, #name, &TCONCAT(_Test_,name)::_RunIt); \
void TCONCAT(_Test_,name)::_Run()

// Register the specified test.  Typically not used directly, but
// invoked via the macro expansion of TEST.
extern bool RegisterTest(const char* base, const char* name, void (*func)());


} // namespace test
} // namespace middleware

#endif
