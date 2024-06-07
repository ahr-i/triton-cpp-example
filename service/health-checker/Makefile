# Compiler and flags settings
CXX = g++
CXXFLAGS = -std=c++14 -Iinclude -O2

# Target executable file
TARGET = server

# List of source files
SRCS = main.cpp \
       health_checker.cpp \
       server.cpp \
       service_handler.cpp \
       setting.cpp

# List of object files
OBJS = $(SRCS:.cpp=.o)

# Libraries to link
LIBS = -lcpprest -lboost_system -lssl -lcrypto

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
main.o: main.cpp server.h setting.h
health_checker.o: health_checker.cpp health_checker.h service_handler.h
server.o: server.cpp server.h health_checker.h
service_handler.o: service_handler.cpp service_handler.h
setting.o: setting.cpp setting.h
