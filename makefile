CXX = g++
CXXFLAGS = -std=c++17
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SRCS = main.cpp Graph.cpp Tree.cpp MSTAlgorithmFactory.cpp Pipeline.cpp ActiveObject.cpp
OBJS = $(SRCS:.cpp=.o)

TARGET = program

all: $(TARGET)
	./$(TARGET)
	
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
