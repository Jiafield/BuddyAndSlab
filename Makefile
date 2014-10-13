all: *.cpp
	g++ -g -Wall -o P2 *.cpp
clean:
	rm -f *.o P2 *~
