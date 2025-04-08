# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11

# Target executable
TARGET = test

# Source files
SRCS = Route.cpp test_routing.cpp VectorRoutingProtocol.cpp ../addressing/Addressing.cpp ../../utils/packet_header.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default rule
all: $(TARGET)

# Linking rule
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilation rule
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
