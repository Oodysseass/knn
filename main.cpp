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
        std::cout << "Error in opening of the file" << std::endl;

    int numNeighbors = atoi(argv[2]);
    int numOfCorpus;
    int dimensions;
    dataFile >> numOfCorpus >> dimensions;
    
    double* corpusPoints = new double[numOfCorpus * dimensions];

    for (int i = 0; i < numOfCorpus * dimensions; i++)
        dataFile >> corpusPoints[i];

    // this is just temporary
    int numOfQuery = 2;
    double* queryPoints = new double[numOfQuery * dimensions];

    for (int i = 0; i < numOfQuery * dimensions; i++)
        queryPoints[i] = corpusPoints[i];
    //

    Knnresult* knnresult = kNN(queryPoints, corpusPoints, numOfQuery, numOfCorpus, dimensions, numNeighbors);

    return 0;
}