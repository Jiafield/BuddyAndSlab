all: *.cpp
	g++ -g -Wall -o P2 *.cpp -std=gnu++0x -lboost_regex
clean:
	rm -f *.o P2 *~
