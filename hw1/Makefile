CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++17

EXECUTABLES = iter_improv sim_anneal tabu_search
SOURCES = iter_improv.cpp sim_anneal.cpp tabu_search.cpp
HEADERS = flowshop_scheduler.hpp

all: $(EXECUTABLES)

$(EXECUTABLES): % : %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(EXECUTABLES)
