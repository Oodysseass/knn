#CC = mpic++
#CFLAGS = -c -O3
#LDFLAGS = -lgsl -lgslcblas
#SOURCES = mainSequential.cpp knn.cpp
#OBJECTS = $(SOURCES:.cpp=.o)
#EXECUTABLE = mainSeq
#
#all: $(EXECUTABLE)
#
#$(EXECUTABLE): $(OBJECTS)
#	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
#
#.cpp.o:
#	$(CC) $(CFLAGS) $< -o $@
#
#clean:
#	rm -f $(OBJECTS) $(EXECUTABLE)
#
CC = mpic++
CFLAGS = -c -O3
LDFLAGS = -lgsl -lgslcblas

SOURCES_SEQ = mainSequential.cpp knn.cpp
OBJECTS_SEQ = $(SOURCES_SEQ:.cpp=.o)
EXECUTABLE_SEQ = mainSeq

SOURCES_MPI = mainMPI.cpp knn.cpp
OBJECTS_MPI = $(SOURCES_MPI:.cpp=.o)
EXECUTABLE_MPI = mainMPI

SOURCES_PROD = fileProducer.cpp
OBJECTS_PROD = $(SOURCES_PROD:.cpp=.o)
EXECUTABLE_PROD = fileProducer

all: $(EXECUTABLE_SEQ) $(EXECUTABLE_MPI) $(EXECUTABLE_PROD)

$(EXECUTABLE_SEQ): $(OBJECTS_SEQ)
	$(CC) $(OBJECTS_SEQ) $(LDFLAGS) -o $@

$(EXECUTABLE_MPI): $(OBJECTS_MPI)
	$(CC) $(OBJECTS_MPI) $(LDFLAGS) -o $@

$(EXECUTABLE_PROD): $(OBJECTS_PROD)
	$(CC) $(OBJECTS_PROD) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(EXECUTABLE_SEQ) $(EXECUTABLE_MPI) $(EXECUTABLE_PROD) *.o	