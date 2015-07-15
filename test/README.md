## introduce how to build CppSQLite3Demo

g++ -c -g -I. -I ./include  -gstabs -Wno-write-strings -Werror -std=c++11 CppSQLite3Demo.cpp
g++ -o test_sqlite CppSQLite3Demo.o -L. ./lib/libmiddleware.so ./lib/libsecrypto.so ./lib/libsqlite3.so
rm -fr CppSQLite3Demo.o
