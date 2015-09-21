# C++ Simple Makefile

all:
	g++ -o cxx11 cxx11.cc -std=c++11 -pthread
	g++ -o MultiProducerSingleConsumer MultiProducerSingleConsumer.cc -std=c++11 -pthread
	g++ -o MultiProducerMultiConsumer MultiProducerMultiConsumer.cc -std=c++11 -pthread
	g++ -o SingleProducerSingleConsumer SingleProducerSingleConsumer.cc -std=c++11 -pthread
	g++ -o SingleProducerMultiConsumer SingleProducerMultiConsumer.cc -std=c++11 -pthread

clean:
	rm -fr cxx11
	rm -fr MultiProducerSingleConsumer
	rm -fr MultiProducerMultiConsumer
	rm -fr SingleProducerSingleConsumer
	rm -fr SingleProducerMultiConsumer

