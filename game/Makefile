CC=g++
CFLAGS=-c -W
LDFLAGS=
SOURCES=main.cpp game.cpp reinforce.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=race-track

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	rm -rf $(OBJECTS)
	mv $(EXECUTABLE) ../

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

