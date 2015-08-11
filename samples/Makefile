# C++ Client Makefile
BUILD_LEVEL ?= DEBUG

HOME=/usr/local/middleware

INCLUDE +=\
          -I ../include \

ifeq ($(strip $(BUILD_LEVEL)), RELEASE)
	CFLAGS= -g -O2 $(INCLUDE) -gstabs -Wno-write-strings -Werror -std=c++0x -DRELEASE
endif
ifeq ($(strip $(BUILD_LEVEL)), DEBUG)
	CFLAGS= -g -O0 $(INCLUDE) -gstabs -Wno-write-strings -Werror -std=c++0x -DDEBUG
endif

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
