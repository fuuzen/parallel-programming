CXX = g++

CXXFLAGS = -std=c++20 -Wall -Wextra -march=native -mtune=native

SRCS = gemm.cpp naive.cpp

OBJS = $(SRCS:.cpp=.o)

naive: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean