# Compiler and flags settings
CXX = g++
CXXFLAGS = -std=c++14 -Iinclude -O2

# Target executable file
TARGET = server

# List of source files
SRCS = main.cpp \
       gateway_request_handler.cpp \
       gateway_server.cpp \
       payment_client.cpp \
       scheduler_client.cpp \
       setting.cpp \
       server.cpp

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
main.o: main.cpp gateway_server.h setting.h
gateway_request_handler.o: gateway_request_handler.cpp gateway_request_handler.h scheduler_client.h payment_client.h
gateway_server.o: gateway_server.cpp gateway_server.h gateway_request_handler.h
payment_client.o: payment_client.cpp payment_client.h
scheduler_client.o: scheduler_client.cpp scheduler_client.h
setting.o: setting.cpp setting.h
server.o: server.cpp server.h gateway_request_handler.h
