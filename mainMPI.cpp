#include <iostream>
#include <fstream>
#include <cstdlib>
#include <mpi.h>
#include "knn.h"

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cout << "Usage: ./main <filename> <numberofneighbors>" << std::endl;
        return 1;
    }

    std::ifstream dataFile(argv[1]);
    if (!dataFile)
    {
        std::cout << "Error in opening of the file" << std::endl;
        return 1;
    }

    int selfTID, numTasks;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numTasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &selfTID);

    int numNeighbors = atoi(argv[2]);
    int numOfCorpus;
    int dimensions;
    dataFile >> numOfCorpus >> dimensions;

    int processNumPoints = numOfCorpus / numTasks;
    if (selfTID == numTasks - 1)
        processNumPoints += numOfCorpus % numTasks;

    double* points = new double[processNumPoints * dimensions];

    int lineNumber = 0;
    std::string line;
    getline(dataFile, line);
    while (lineNumber < numOfCorpus / numTasks * selfTID)
    {
        getline(dataFile, line);
        lineNumber++;
    }

    for (int i = 0; i < processNumPoints * dimensions; i++)
        dataFile >> points[i];

    distrallkNN(points, processNumPoints, numOfCorpus, dimensions, numNeighbors);

    MPI_Finalize();

    return 0;
}