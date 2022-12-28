CC = mpic++
CFLAGS = -O3
LDFLAGS = -lgsl -lgslcblas
SOURCES = main.cpp
EXECUTABLE = main

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

clean:
	rm -f $(EXECUTABLE)