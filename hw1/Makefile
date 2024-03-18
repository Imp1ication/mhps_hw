CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11

EXECUTABLES = main iter_improv sim_anneal
SOURCES = main.cpp iter_improv.cpp sim_anneal.cpp
HEADERS = flowshop_scheduler.hpp

all: $(EXECUTABLES)

$(EXECUTABLES): % : %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(EXECUTABLES)
