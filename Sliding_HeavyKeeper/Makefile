main:main.o
	g++ -o main main.o
main.o:main.cpp BOBHash32.h BOBHash64.h CSS.h heavykeeper.h LossyCounting.h params.h spacesaving.h ssummary.h
	g++ -g -c main.cpp
clean:
	rm -f *.o main