#compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -pedantic -Wall -Wextra

#name of the final executable
TARGET = wordgen

#object files
OBJS = WordGen.o generate_functions.o

#default output when make is run
all: $(TARGET)

#rule to build the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

#rule to compile WordGen.cpp
WordGen.o: WordGen.cpp generate_functions.h
	$(CXX) $(CXXFLAGS) -c WordGen.cpp

#rule to compile generate_functions.cpp
generate_functions.o: generate_functions.cpp generate_functions.h
	$(CXX) $(CXXFLAGS) -c generate_functions.cpp

#clean target to remove executable
clean:
	rm -f $(TARGET) $(OBJS)
