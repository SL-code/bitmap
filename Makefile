default: main.o bitmap.o
	g++ -o bitmap main.o bitmap.o

bitmap.o: bitmap.cpp bitmap.h
	g++ -c bitmap.cpp

main.o: main.cpp
	g++ -c main.cpp
