CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
INCLUDES = -I../My_graph

SRC = main.cpp Dijkstra.cpp ../My_graph/Edge.cpp
OBJ = main.o Dijkstra.o Edge.o
TARGET = dijkstra

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(OBJ)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

Dijkstra.o: Dijkstra.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

Edge.o: ../My_graph/Edge.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f *.o $(TARGET)
