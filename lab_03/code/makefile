CC=g++
CFLAGS=-c -Wall


all: app


app: main.o array.o sort.o timeTests.o
	$(CC) main.o array.o sort.o timeTests.o -o app


main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp


array.o: array.cpp
	$(CC) $(CFLAGS) array.cpp


sort.o: sort.cpp
	$(CC) $(CFLAGS) sort.cpp


timeTests.o: timeTests.cpp
	$(CC) $(CFLAGS) timeTests.cpp


clean:
	rm -rf *.o app