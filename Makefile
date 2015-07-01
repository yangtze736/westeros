# C++ Client Makefile

# if you create new source folder(include cpp) please add to below like
VPATH = src

# if you have new inc folder need to input, add in below like 
INCLUDE +=\
          -I. \
          -I ./include \
		  -I ./parser/json/include \

CFLAGS= -g $(INCLUDE) -gstabs -Wno-write-strings -Werror -fPIC -shared -std=c++11
CC = g++

# if you have new lib need to link, add in below like
LIBSECRYPTO=./lib/libsecrypto.so
LIBSQLITE=./lib/libsqlite3.so
LIB+=-L. -lz -lcurl -lrt -lcrypto -lssl -lstdc++ -lpthread $(LIBSQLITE) $(LIBJSON) $(LIBSECRYPTO)

LIB_FLAGS = -pthread -fPIC -shared

# need *.o to generate test_client
LIB_MIDDLEWARE = libmiddleware.so

LIB_DEP = interface.o versionInfo.o mfcFile.o \
		  middleware.o generateUrl.o urlhandle.o \
		  baseFunc.o httpClient.o CppSQLite3.o \
		  parser.o static.o configure.o \
		  createObject.o readObject.o \
		  fileUpload.o fileDownload.o \

all: $(LIB_MIDDLEWARE)
# the rule how to make *.o
.c.o:
	$(CC) -c $(CFLAGS) $<

.cpp.o:
	$(CC) -c $(CFLAGS) $<

$(LIB_MIDDLEWARE): $(LIB_DEP)
	$(CC) -o $(LIB_MIDDLEWARE) $(LIB_DEP) $(LIB_FLAGS) $(LIB)
	cp -rf include/*.h samples/include/;cp -rf include/*.h daemon/include/;mv $(LIB_MIDDLEWARE) lib/;cp lib/*.so samples/lib/;cp lib/*.so daemon/lib/;rm -rf ./parser/json/*.o;rm -fr *.o

clean:
	rm -rf ./lib/libjson.so ./lib/libmiddleware.so
	rm -rf ./samples/lib/*.so

dep:
	$(CC) -MM *.c *.cpp

log:
	git log '--pretty=format:%ad %s' --date=short > Changelog

# describe how many file the .o file needed to build
interface.o:src/interface.cpp
baseFunc.o:src/baseFunc.cpp include/baseFunc.h
mfcFile.o:src/mfcFile.cpp include/mfcFile.h
middlewareLog.o:src/middlewareLog.cpp include/middlewareLog.h
static.o:src/static.cpp include/static.h
httpClient.o:src/httpClient.cpp include/httpClient.h
middleware.o:src/middleware.cpp include/middleware.h
parser.o:src/parser.cpp include/parser.h
configure.o:src/configure.cpp include/configure.h
versionInfo.o:src/versionInfo.cpp include/versionInfo.h
generateUrl.o:src/generateUrl.cpp include/generateUrl.h
CppSQLite3.o:src/CppSQLite3.cpp include/CppSQLite3.h
createObject.o:src/createObject.cpp include/createObject.h
readObject.o:src/readObject.cpp include/readObject.h
fileUpload.o:src/fileUpload.cpp include/fileUpload.h
fileDownload.o:src/fileDownload.cpp include/fileDownload.h
urlhandle.o:src/urlhandle.cpp include/urlhandle.h
