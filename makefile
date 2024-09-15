CXX = g++
CXXFLAGS = -std=c++17
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SRCS = main.cpp Graph.cpp Tree.cpp MSTAlgorithmFactory.cpp Pipeline.cpp ActiveObject.cpp
OBJS = $(SRCS:.cpp=.o)
CLIENT_COMMANDS = "init"
TARGET = program

all: $(TARGET)
	./$(TARGET) &  # Start the server in the background
	sleep 2        # Wait for the server to start
	cmd.exe /c start wsl.exe -e nc localhost 9034
	

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
	pkill -f $(TARGET) || true

.PHONY: all clean
