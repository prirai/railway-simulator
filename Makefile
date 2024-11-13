CXX = g++
CXX_DEBUG_FLAGS = -std=c++20 -Wall -g -O0 -DDEBUG
CXX_OPTIMIZED_FLAGS = -std=c++20 -Wall -O3 -DNDEBUG

all: debug optimized

debug: CXXFLAGS = $(CXX_DEBUG_FLAGS)
debug: lib/librailway.a test_railway_debug

optimized: CXXFLAGS = $(CXX_OPTIMIZED_FLAGS)
optimized: test_railway_optimized

lib/librailway.a: railway_lib.o
	$(AR) rcs $@ $^

test_railway_debug: test_railway.o lib/librailway.a
	$(CXX) $(CXX_DEBUG_FLAGS) $^ -o $@

test_railway_optimized: test_railway.o railway_lib.o
	$(CXX) $(CXX_OPTIMIZED_FLAGS) $^ -o $@

test_railway.o: test_railway.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

railway_lib.o: lib/railway_lib.cpp lib/railway_lib.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf *.o lib/*.o lib/*.a test_railway_debug test_railway_optimized

.PHONY: clean debug optimized