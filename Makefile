CXX ?= g++
CXXFLAGS ?= -O2 -std=c++17
INCLUDES ?= -I./tdzdd/include

TARGET = path_count
SRCS = main.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(SRCS)

clean:
	rm -f $(TARGET)
