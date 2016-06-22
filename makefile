CC=g++
CFLAGS= -c -Wall -Wno-sign-compare -std=c++11 -O3
LDLIBS= 
SOURCES=main.cpp types.cpp neuron.cpp neuralnetwork.cpp training.cpp pooling.cpp convolutionalneuralnetwork.cpp mnist.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=net


all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE) $(LDLIBS)


#$@ = name of target, $< = name of prerequisite
%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)
