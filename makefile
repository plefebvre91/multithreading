CXX=g++
CXXFLAGS=--std=c++20 -Wall -Wpedantic

all: producer-consumer reader-writer

producer-consumer: producer_consumer.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

reader-writer: reader_writer.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	@rm reader-writer producer-consumer *.o *~
