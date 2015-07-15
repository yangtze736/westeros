VPATH = src
INCLUDE=\
	-I ./include\

GCC = g++
CFLAG = -g -Wall -fPIC -shared $(INCLUDE)
objects = json_reader.o json_value.o json_writer.o

all: libjson.so
libjson.so:$(objects)
	ar rc $@ $(objects)
	mv $@ ../../lib/

$(objects): %.o: %.cpp
	$(GCC) -c $(CFLAG) $< -o $@

clean:
	rm -f *.o
	rm -f ../../lib/libjson.so
