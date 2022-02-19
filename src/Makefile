CC = g++
OPT = -O3
LIBS = -static-libstdc++
PLIBS = -pthread

all: main parallel

main: main.cpp
	$(CC) -o ../bin/hw2 main.cpp $(OPT) $(LIBS)

parallel: parallel.cpp
	$(CC) -o ../bin/hw2_parallel parallel.cpp $(OPT) $(LIBS) $(PLIBS)

clean:
	rm -f ../bin/hw2 ../bin/hw2_parallel
