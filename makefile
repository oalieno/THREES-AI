all:
	g++-7 -std=c++17 -O3 -g -Wall -fmessage-length=0 -o threes threes.cpp
clean:
	rm threes
