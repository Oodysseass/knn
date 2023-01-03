CC = mpic++
CFLAGS = -c -O3
LDFLAGS = -lgsl -lgslcblas
SOURCES = mainSequential.cpp knn.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = mainSeq

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
