CXX = g++
CXXFLAGS = -std=c++11 -Wall

SRCS = main.cpp Registro.cpp Buffer.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = Registro.h Buffer.h
TARGET = programa

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)
