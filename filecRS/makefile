CC = g++ -c -std=c++11
GG = g++ -std=c++11

all: test
	./test

test: testRsFile.cc rs.o
	$(GG) -o test testRsFile.cc rs.o 

rs.o: rs.cc rs.h
	$(CC) -o rs.o rs.cc

# test: testRsFile.cc rs.o rsFile.o
	# $(GG) -o test testRsFile.cc rs.o rsFile.o

# rsFile.o: rsFile.cc rsFile.h 
	# $(CC) -o rsFile.o rsFile.cc 

# rs.o: rs.c rs.h
	# $(CC) -o rs.o rs.c

clean:
	rm *.o test


