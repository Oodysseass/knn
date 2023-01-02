#include <iostream>
#include <mpi.h>
#include "knn.hpp"

int partition(double *A, int low, int high, int *ids) {
    double pivot = A[high];
    int i = (low - 1);

    for (int j = low; j < high; j++) {
        if (A[j] < pivot) {
            i++;
            std::swap(A[i], A[j]);
            std::swap(ids[i], ids[j]);
        }
    }

    std::swap(A[i + 1], A[high]);
    std::swap(ids[i + 1], ids[high]);
    return (i + 1);
}

// variation of quickSort that changes array ids too
// according to the sorting of array A
void quickSort(double *A, int low, int high, int *ids) {
    if (low < high) {
        int pivot = partition(A, low, high, ids);
        quickSort(A, low, pivot - 1, ids);
        quickSort(A, pivot + 1, high, ids);
    }
}

void compareResults(Knnresult *result, Knnresult *temp, int n, int k, int index, int coefficient)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < k; j++)
        {
            // if it's further than the current furthest
            // don't even check the next ones
            if (temp->ndist[i * k + j] >= result->ndist[(i + 1) * k - 1])
                break;

            // else replace the furthest
            // fix indexes and resort
            else
            {
                result->nidx[(i + 1) * k - 1] = temp->ndist[i * k + j] + index * coefficient;
                result->ndist[(i + 1) * k - 1] = temp->ndist[i * k + j];
                quickSort(&result->ndist[i * k], 0, k - 1, &result->nidx[i * k]);
            }
        }
    }
}

Knnresult distrallkNN(double *query, int n, int totalPoints, int d, int k)
{
    int selfTID, numTasks;
    MPI_Comm_size(MPI_COMM_WORLD, &numTasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &selfTID);

    MPI_Request sendReq, recvReq;
    MPI_Status status;

    // the process with selfTID == numTasks - 1 has maxSize query points
    // while every other process has averageSize query points
    int averageSize = totalPoints / numTasks;
    int maxSize = totalPoints / numTasks + totalPoints % numTasks;
    int indexConverter = selfTID;

    Knnresult result = Knnresult(n, d);

    // pointers for sending, receiving and exchanging locations
    double *corpus = new double[maxSize * d + 1];
    double *incoming = new double[maxSize * d + 1];
    double *temp;
    std::copy(query, query + n * d, corpus);

    // we also send the actual number of points
    int actualSize = n;
    corpus[maxSize * d] = actualSize;

    // prepare to send and receive while we calculate
    MPI_Isend(corpus, maxSize * d + 1, MPI_DOUBLE, (selfTID + 1) % numTasks, 1, MPI_COMM_WORLD, &sendReq);
    MPI_Irecv(incoming, maxSize * d + 1, MPI_DOUBLE, (selfTID - 1 + numTasks) % numTasks, 1, MPI_COMM_WORLD, &recvReq);

    result = kNN(query, corpus, n, n, d, k);

    // correcting indexes to be global
    for (int i = 0; i < n * k; i++)
        result.nidx[i] += indexConverter * averageSize;

    for (int i = 1; i < numTasks; i++)
    {
        // making sure communication has ended
        MPI_Wait(&sendReq, &status);
        MPI_Wait(&recvReq, &status);

        temp = corpus;
        corpus = incoming;
        incoming = temp;

        actualSize = corpus[maxSize * d];

        // no need to send anything on last iteration
        if (i != numTasks - 1)
        {
            MPI_Isend(corpus, maxSize * d + 1, MPI_DOUBLE, (selfTID + 1) % numTasks, 1, MPI_COMM_WORLD, &sendReq);
            MPI_Irecv(incoming, maxSize * d + 1, MPI_DOUBLE, (selfTID - 1 + numTasks) % numTasks, 1, MPI_COMM_WORLD, &recvReq);
        }

        Knnresult tempResult = kNN(query, corpus, n, actualSize, d, k);
        indexConverter--;
        compareResults(&result, &tempResult, n, k, indexConverter, averageSize);
    }


    return result;
}