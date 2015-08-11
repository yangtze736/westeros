# C++ Client Makefile
BUILD_LEVEL ?= DEBUG

# if you create new source folder(include cpp) please add to below like
VPATH = src
HOME=/usr/local/middleware

# if you have new inc folder need to input, add in below like 
INCLUDE +=\
          -I ./include \

#CFLAGS= -g -O2 $(INCLUDE) -gstabs -Wno-write-strings -Werror -fPIC -shared -std=c++0x
ifeq ($(strip $(BUILD_LEVEL)), RELEASE)
	CFLAGS= -g -O2 $(INCLUDE) -gstabs -Wno-write-strings -Werror -fPIC -shared -std=c++0x -DRELEASE
endif
ifeq ($(strip $(BUILD_LEVEL)), DEBUG)
	CFLAGS= -g -O0 $(INCLUDE) -gstabs -Wno-write-strings -Werror -fPIC -shared -std=c++0x -DDEBUG
endif

CC = g++

# if you have new lib need to link, add in below like
LIBSECRYPTO=$(HOME)/lib/libsecrypto.so
LIBSQLITE=$(HOME)/lib/libsqlite3.so
LIBSNAPPY=$(HOME)/lib/libsnappy.so.1.3.0
LIB+=-L. -lz -lcurl -lrt -lcrypto -lssl -lstdc++ -lpthread $(LIBSQLITE) $(LIBJSON) $(LIBSECRYPTO) $(LIBSNAPPY)

LIB_FLAGS = -fPIC -shared

# need *.o to generate test_client
LIB_MIDDLEWARE = libmiddleware.so

LIB_DEP = interface.o mfcFile.o middleware.o generateUrl.o urlhandle.o \
		  baseFunc.o httpClient.o CppSQLite3.o parser.o static.o configure.o \
		  createObject.o readObject.o fileUpload.o fileDownload.o \
		  compress.o trap.o\

all: check $(LIB_MIDDLEWARE) clean

check:
	cd lib/ && tar zxvf libdependent.tar.gz
	if [ ! -d $(HOME)/lib ];then mkdir -p $(HOME)/lib;fi
	cp -rf lib/* $(HOME)/lib

clean:
	rm -fr lib/lib*.so*
	cp -rf $(LIB_MIDDLEWARE) $(HOME)/lib/
	mv -f $(LIB_MIDDLEWARE) lib/
	rm -fr *.o 

log:
	git log '--pretty=format:%ad %s' --date=short > Changelog

# the rule how to make *.o
.c.o:
	$(CC) -c $(CFLAGS) $<

.cpp.o:
	$(CC) -c $(CFLAGS) $<

$(LIB_MIDDLEWARE): $(LIB_DEP)
	$(CC) -o $(LIB_MIDDLEWARE) $(LIB_DEP) $(LIB_FLAGS) $(LIB)

dep:
	$(CC) -MM *.c *.cpp


# describe how many file the .o file needed to build
interface.o:src/interface.cpp
baseFunc.o:src/baseFunc.cpp include/baseFunc.h
mfcFile.o:src/mfcFile.cpp include/mfcFile.h
log.o:src/log.cpp include/log.h
static.o:src/static.cpp include/static.h
httpClient.o:src/httpClient.cpp include/httpClient.h
middleware.o:src/middleware.cpp include/middleware.h
parser.o:src/parser.cpp include/parser.h
trap.o:src/trap.cpp include/trap.h
configure.o:src/configure.cpp include/configure.h
generateUrl.o:src/generateUrl.cpp include/generateUrl.h
CppSQLite3.o:src/CppSQLite3.cpp include/CppSQLite3.h
createObject.o:src/createObject.cpp include/createObject.h
readObject.o:src/readObject.cpp include/readObject.h
fileUpload.o:src/fileUpload.cpp include/fileUpload.h
fileDownload.o:src/fileDownload.cpp include/fileDownload.h
urlhandle.o:src/urlhandle.cpp include/urlhandle.h
compress.o:src/compress.cpp include/compress.h
