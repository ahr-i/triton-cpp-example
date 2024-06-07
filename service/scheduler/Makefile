# Compiler and flags settings
CXX = g++
CXXFLAGS = -std=c++14 -Iinclude -O2

# Target executable file
TARGET = server

# List of source files
SRCS = main.cpp \
       node_manager.cpp \
       request_handler.cpp \
       scheduler.cpp \
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
main.o: main.cpp scheduler.h setting.h
node_manager.o: node_manager.cpp node_manager.h
request_handler.o: request_handler.cpp request_handler.h
scheduler.o: scheduler.cpp scheduler.h node_manager.h request_handler.h
setting.o: setting.cpp setting.h
