.PHONY: all debug clean

CXX      := g++-7
CXXFLAGS := -std=c++17 -O3
INCLUDE  := -I../flags/include -I../spdlog/include -I../fmt/include
LINK     := -lstdc++fs

threes: threes.cpp *.h
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ $< $(LINK)

all: threes

clean:
	rm threes
