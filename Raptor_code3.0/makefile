CC = g++ -c -std=c++11
GG = g++ -std=c++11

all: test
	./test

test: main.cpp CDistribution.cpp Decode.cpp Encode.cpp Gau_Channel.cpp RandNum1.cpp RandNum.cpp Source.cpp
	$(GG) -o test main.cpp CDistribution.cpp Decode.cpp Encode.cpp Gau_Channel.cpp RandNum1.cpp RandNum.cpp Source.cpp

clean :
	rm *.o test
