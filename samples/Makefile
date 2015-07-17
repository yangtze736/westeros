# C++ Client Makefile

HOME=/usr/local/middleware

INCLUDE +=\
          -I ../include \

#CFLAGS= -g $(INCLUDE) -gstabs -Wwrite-strings -Werror -std=c++11
CFLAGS= -g $(INCLUDE) -gstabs -Wno-write-strings -Werror -std=c++11 
CC = g++

# if you have new lib need to link, add in below like
# LIB+=-L. -lcurl -lz -lrt -lcrypto -lssl -lstdc++ -lpthread -ljsoncpp -lxml
LIB_MIDDLEWARE=$(HOME)/lib/libmiddleware.so
LIB_SECRYPTO=$(HOME)/lib/libsecrypto.so
LIB_SQLITE3=$(HOME)/lib/libsqlite3.so
LIBSNAPPY=$(HOME)/lib/libsnappy.so.1.3.0
LIB+=-L. $(LIB_MIDDLEWARE) $(LIB_SECRYPTO) $(LIB_SQLITE3) $(LIBSNAPPY)

# need *.o to generate test_client
APP = middleware
APPOBJS = middleware.o

all: $(APP)

# the rule how to make *.o
.c.o:
	$(CC) -c $(CFLAGS) $<

.cpp.o:
	$(CC) -c $(CFLAGS) $<

$(APP): $(APPOBJS)
	$(CC) -o $(APP) $(APPOBJS) $(LIB)
	rm -fr *.o

test: $(APP)
	@./test_middleware

check: test

clean:
	rm -rf *.o *.txt *.pem $(APP)

dep:
	$(CC) -MM *.c *.cpp

#log:
#	git log '--pretty=format:%ad %s' --date=short > Changelog

# describe how many file the .o file needed to build
middleware.o:middleware.cpp
