CC = mpic++
CFLAGS = -O3
LDFLAGS = -lcblas
SOURCES = main.cpp
EXECUTABLE = main

$(EXECUTABLE):
	$(CC) $(SOURCES) -o $@ $(CFLAGS) $(LDFLAGS) 

clean:
	rm -f $(EXECUTABLE)