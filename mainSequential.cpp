#include <iostream>
#include <fstream>
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

    int numNeighbors = atoi(argv[2]);
    int numOfCorpus;
    int dimensions;
    dataFile >> numOfCorpus >> dimensions;
    
    double* corpusPoints = new double[numOfCorpus * dimensions];

    for (int i = 0; i < numOfCorpus * dimensions; i++)
        dataFile >> corpusPoints[i];

    // this is just temporary
    int numOfQuery = numOfCorpus;
    double* queryPoints = new double[numOfQuery * dimensions];

    for (int i = 0; i < numOfQuery * dimensions; i++)
        queryPoints[i] = corpusPoints[i];
    //

    double start = MPI_Wtime();
    Knnresult knnresult = kNN(queryPoints, corpusPoints, numOfQuery, numOfCorpus, dimensions, numNeighbors);
    double end = MPI_Wtime();

    std::ofstream outputFile("seqOutput.txt");
    for (int i = 0; i < numOfQuery; i++)
    {
        outputFile << "KNNs of point " << i << ":" << std::endl;
        for (int j = 0; j < numNeighbors; j++)
            outputFile << "Neighbor " << j + 1 << ": " << "ID: " <<
                knnresult.nidx[i * numNeighbors + j] << ", Dist: " <<
                knnresult.ndist[i * numNeighbors + j] << std::endl;
        outputFile << "---------------" << std::endl;
    }

    std::cout << "Time needed: " << end - start << "s." << std::endl;

    delete[] corpusPoints;
    delete[] queryPoints;

    return 0;
}