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
    MPI_Status status;
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

    Knnresult myResult = distrallkNN(points, processNumPoints, numOfCorpus, dimensions, numNeighbors);
    Knnresult overAll = Knnresult(numOfCorpus, numNeighbors);

    if (selfTID == 0)
    {
        for (int i = 0; i < processNumPoints * numNeighbors; i++)
        {
            overAll.nidx[i] = myResult.nidx[i];
            overAll.ndist[i] = myResult.ndist[i];
        }
        for (int i = 1; i < numTasks; i++)
        {
            if (i != numTasks - 1)
            {
                Knnresult temp = Knnresult(processNumPoints, numNeighbors);
                MPI_Recv(temp.nidx, processNumPoints * numNeighbors, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
                MPI_Recv(temp.ndist, processNumPoints * numNeighbors, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &status);
                for (int j = 0; j < processNumPoints * numNeighbors; j++)
                {
                    overAll.nidx[i * processNumPoints * numNeighbors + j] = temp.nidx[j];
                    overAll.ndist[i * processNumPoints * numNeighbors + j] = temp.ndist[j];
                }
            }
            else
            {
                Knnresult temp = Knnresult(processNumPoints + numOfCorpus % numTasks, numNeighbors);
                MPI_Recv(temp.nidx, (processNumPoints + numOfCorpus % numTasks) * numNeighbors, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
                MPI_Recv(temp.ndist, (processNumPoints + numOfCorpus % numTasks) * numNeighbors, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &status);
                for (int j = 0; j < (processNumPoints + numOfCorpus % numTasks) * numNeighbors; j++)
                {
                    overAll.nidx[i * processNumPoints * numNeighbors + j] = temp.nidx[j];
                    overAll.ndist[i * processNumPoints * numNeighbors + j] = temp.ndist[j];
                }
            }
        }
    }
    else
    {
        MPI_Send(myResult.nidx, processNumPoints * numNeighbors, MPI_INT, 0, 1, MPI_COMM_WORLD);
        MPI_Send(myResult.ndist, processNumPoints * numNeighbors, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    if (selfTID == 0)
    {
        for (int i = 0; i < numOfCorpus; i++)
        {
            std::cout << "KNNs of point " << i << ":" << std::endl;
            for (int j = 0; j < numNeighbors; j++)
                std::cout << "Neighbor " <<  j + 1 << ": " << "ID: " <<
                    overAll.nidx[i * numNeighbors + j] << ", Dist: " <<
                    overAll.ndist[i * numNeighbors + j] << std::endl;
            std::cout << "---------------" << std::endl;
        }
    }

    delete[] points;

    return 0;
}