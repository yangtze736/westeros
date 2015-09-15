# C++ Client Makefile
HOME=/usr/local/middleware

INCLUDE +=\
          -I ../../include \

CFLAGS= -g -O2 $(INCLUDE) -gstabs -Wno-write-strings -Werror -std=c++0x

CC = g++

# if you have new lib need to link, add in below like
LIB_MIDDLEWARE=$(HOME)/lib/libmiddleware.so
LIB+=-L. $(LIB_MIDDLEWARE)

TARGETS = middleware
SRCS = middleware.cpp

all:
	$(CC) -o $(TARGETS) $(SRCS) $(CFLAGS) $(LIB)

clean:
	rm -fr $(TARGETS) test.db
