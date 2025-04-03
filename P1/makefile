CXX = g++
CXXFLAGS = -std=c++11 -pthread -Wall -Wextra
TARGET = dining_philosophers
SRC = dining_philosophers.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)