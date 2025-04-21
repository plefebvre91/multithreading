CXX=g++
CXXFLAGS=--std=c++20 -Wall -Wpedantic

all: producer-consumer

producer-consumer: producer_consumer.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@
