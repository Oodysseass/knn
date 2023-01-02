#include <iostream>
#include <mpi.h>
#include "knn.hpp"

Knnresult distrallkNN(double *query, int n, int totalPoints, int d, int k)
{
    int selfTID, numTasks;
    MPI_Comm_size(MPI_COMM_WORLD, &numTasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &selfTID);

    MPI_Request sendReq, recvReq;
    MPI_Status status;

    int maxSize = totalPoints / numTasks + totalPoints % numTasks;

    Knnresult result = Knnresult(n, d);

    double *corpus = new double[maxSize * d + 1];
    double *incoming = new double[maxSize * d + 1];
    double *temp;
    std::copy(query, query + n * d, corpus);

    int actualSize = n;
    corpus[maxSize * d] = actualSize;

    MPI_Isend(corpus, maxSize * d + 1, MPI_DOUBLE, (selfTID + 1) % numTasks, 1, MPI_COMM_WORLD, &sendReq);
    MPI_Irecv(incoming, maxSize * d + 1, MPI_DOUBLE, (selfTID - 1 + numTasks) % numTasks, 1, MPI_COMM_WORLD, &recvReq);

    result = kNN(query, corpus, n, n, d, k);

    for (int i = 0; i < numTasks; i++)
    {
        MPI_Wait(&sendReq, &status);
        MPI_Wait(&recvReq, &status);

        temp = corpus;
        corpus = incoming;
        incoming = temp;

        actualSize = corpus[maxSize * d];

        
    }


    return result;
}