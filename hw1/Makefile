CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11

SRCS = main.cpp
HEADERS = flowshop_scheduler.hpp iter_improv.hpp
TARGET = main

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS)

clean:
	rm -f $(TARGET)
