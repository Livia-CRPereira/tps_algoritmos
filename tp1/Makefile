CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Wpedantic
TARGET := tp01
SRCS := main.cpp bfs.cpp

.PHONY: all eval clean

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

eval: $(TARGET)
	@sh tests/evaluate.sh ./$(TARGET)

clean:
	rm -f $(TARGET)