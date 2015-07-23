# C++ Client Makefile

HOME=/usr/local/middleware

INCLUDE +=\
          -I ../include \

CFLAGS= -g $(INCLUDE) -gstabs -Wno-write-strings -Werror -std=c++0x
CC = g++

# if you have new lib need to link, add in below like
LIB_MIDDLEWARE=$(HOME)/lib/libmiddleware.so
LIB+=-L. $(LIB_MIDDLEWARE)

APP = middleware
APPOBJS = middleware.cpp

all:
	$(CC) -o $(APP) $(APPOBJS) $(CFLAGS) $(LIB)
	rm -fr *.o
