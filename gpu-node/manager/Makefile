# Compiler settings
CXX = g++
CXXFLAGS = -std=c++14 -Iinclude -O2

# Library and linker settings
LDFLAGS = -lcpprest -lboost_system -lssl -lcrypto

# Source files and object files settings
SRCS = main.cpp manager.cpp ping_handler.cpp request_handler.cpp server.cpp setting.cpp triton_client.cpp
OBJS = $(SRCS:.cpp=.o)

# Output executable name
TARGET = server

# Default rule: build everything
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Rule to build object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)

# Debug build rule
debug: CXXFLAGS += -g
debug: clean $(TARGET)

.PHONY: all clean debug
