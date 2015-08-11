## introduce how to build CppSQLite3 demo

1) build object file: CppSQLite3Demo.o
g++ -c -g -I. -I ../include  -gstabs -Wno-write-strings -Werror -std=c++11 CppSQLite3Demo.cpp

2) build execute file: test_sqlite
g++ -o test_sqlite CppSQLite3Demo.o -L. /usr/local/middleware/lib/libmiddleware.so \
                                        /usr/local/middleware/lib/libsecrypto.so \
                                        /usr/local/middleware/lib/libsqlite3.so \
                                        /usr/local/middleware/lib/libsnappy.so.1.3.0
