# C++ Client Makefile

INCLUDE +=\
          -I. \
          -I ./include \

#CFLAGS= -g $(INCLUDE) -gstabs -Wwrite-strings -Werror -std=c++11
CFLAGS= -g $(INCLUDE) -gstabs -Wno-write-strings -Werror -std=c++11
CC = g++

# if you have new lib need to link, add in below like
# LIB+=-L. -lcurl -lz -lrt -lcrypto -lssl -lstdc++ -lpthread -ljsoncpp -lxml
LIB_MIDDLEWARE=./lib/libmiddleware.so
LIB_SECRYPTO=./lib/libsecrypto.so
LIB_SQLITE3=./lib/libsqlite3.so
LIB+=-L. $(LIB_MIDDLEWARE) $(LIB_SECRYPTO) $(LIB_SQLITE3)

# need *.o to generate test_client
APP = test_middleware
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
