# Compiler and flags settings
CXX = g++
CXXFLAGS = -std=c++14 -Iinclude -O2

# Target executable file
TARGET = server

# List of source files
SRCS = main.cpp \
       request_handler.cpp \
       server.cpp \
       setting.cpp \
       web_server.cpp

# List of object files
OBJS = $(SRCS:.cpp=.o)

# Libraries to link
LIBS = -lcpprest -lboost_system -lssl -lcrypto -lpng

# Default rule: create the executable file
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

# Rule to create each .o file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule: delete object files and executable file
clean:
	rm -f $(OBJS) $(TARGET)

# File dependencies
main.o: main.cpp web_server.h setting.h
request_handler.o: request_handler.cpp request_handler.h
server.o: server.cpp server.h
setting.o: setting.cpp setting.h
web_server.o: web_server.cpp web_server.h
