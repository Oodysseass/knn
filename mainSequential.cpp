#include <iostream>
#include <fstream>
#include <string>
#include "knn.hpp"


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

    Knnresult knnresult = kNN(queryPoints, corpusPoints, numOfQuery, numOfCorpus, dimensions, numNeighbors);

    for (int i = 0; i < numOfQuery; i++)
    {
        std::cout << "KNNs of point " << i << ":" << std::endl;
        for (int j = 0; j < numNeighbors; j++)
            std::cout << j + 1 << ": " << knnresult.nidx[i * numNeighbors + j] <<
                ", Dist: " << knnresult.ndist[i * numNeighbors + j] << std::endl;
        std::cout << "---------------" << std::endl;
    }

    delete[] corpusPoints;
    delete[] queryPoints;

    return 0;
}