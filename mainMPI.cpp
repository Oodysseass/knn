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

    double start = MPI_Wtime();
    Knnresult result = distrallkNN(points, processNumPoints, numOfCorpus, dimensions, numNeighbors);
    double end = MPI_Wtime();

    double comStart = MPI_Wtime();
    if (selfTID == 0)
    {
        std::ofstream outputFile("mpiOutput.txt");
        for (int i = 0; i < processNumPoints; i++)
        {
            outputFile << "KNNs of point " << i << ":" << std::endl;
            for (int j = 0; j < numNeighbors; j++)
            {
                outputFile << "Neighbor " << j + 1 << ": "
                           << "ID: " << result.nidx[i * numNeighbors + j]
                           << ", Dist: " << result.ndist[i * numNeighbors + j]
                           << std::endl;
            }
            outputFile << "---------------" << std::endl;
        }
        for (int i = 1; i < numTasks; i++)
        {
            if (i != numTasks - 1)
            {
                MPI_Recv(result.nidx, processNumPoints * numNeighbors, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
                MPI_Recv(result.ndist, processNumPoints * numNeighbors, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &status);
                for (int j = 0; j < processNumPoints; j++)
                {
                    outputFile << "KNNs of point " << i * processNumPoints + j << ":" << std::endl;
                    for (int k = 0; k < numNeighbors; k++)
                    {
                        outputFile << "Neighbor " << k + 1 << ": "
                                   << "ID: " << result.nidx[j * numNeighbors + k]
                                   << ", Dist: " << result.ndist[j * numNeighbors + k]
                                   << std::endl;
                    }
                    outputFile << "---------------" << std::endl;
                }
            }
            else
            {
                result = Knnresult(processNumPoints + numOfCorpus % numTasks, numNeighbors);
                MPI_Recv(result.nidx, (processNumPoints + numOfCorpus % numTasks) * numNeighbors, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
                MPI_Recv(result.ndist, (processNumPoints + numOfCorpus % numTasks) * numNeighbors, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &status);
                for (int j = 0; j < processNumPoints + numOfCorpus % numTasks; j++)
                {
                    outputFile << "KNNs of point " << i * processNumPoints + j << ":" << std::endl;
                    for (int k = 0; k < numNeighbors; k++)
                    {
                        outputFile << "Neighbor " << k + 1 << ": "
                                   << "ID: " << result.nidx[j * numNeighbors + k]
                                   << ", Dist: " << result.ndist[j * numNeighbors + k]
                                   << std::endl;
                    }
                    outputFile << "---------------" << std::endl;
                }
            }
        }
    }
    else
    {
        MPI_Send(result.nidx, processNumPoints * numNeighbors, MPI_INT, 0, 1, MPI_COMM_WORLD);
        MPI_Send(result.ndist, processNumPoints * numNeighbors, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
    }
    double comEnd = MPI_Wtime();

    MPI_Finalize();

    if (selfTID == 0)
    {
        std::cout << "Time needed for gathering of results: " << comEnd - comStart << "s." << std::endl;
        std::cout << "Total time needed for calculation: " << end - start << "s." << std::endl;
    }


    delete[] points;

    return 0;
}