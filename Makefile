CC = mpic++
CFLAGS = -O3
LDFLAGS = -lgsl -lgslcblas
SOURCES = mainSequential.cpp
EXECUTABLE = mainSeq

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

clean:
	rm -f $(EXECUTABLE)