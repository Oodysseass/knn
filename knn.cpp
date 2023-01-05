#include "knn.h"
#include "utilities.hpp"

int partition(double A[], int low, int high)
{
    double pivot = A[high];
    int i = low;
    for (int j = low; j <= high - 1; j++)
    {
        if (A[j] <= pivot)
        {
            std::swap(A[i], A[j]);
            i++;
        }
    }
    std::swap(A[i], A[high]);
    return i;
}

double kthSmallest(double A[], int low, int high, int k)
{
    if (k > 0 && k <= high - low + 1)
    {

        int index = partition(A, low, high);

        if (index - low == k - 1)
            return A[index];

        if (index - low > k - 1)
            return kthSmallest(A, low, index - 1, k);

        return kthSmallest(A, index + 1, high, k - index + low - 1);
    }

    return INT_MAX;
}

Knnresult kNN(double *query, double *corpus, int m, int n, int d, int k)
{
    Knnresult result = Knnresult(m, k);
    result.m = m;
    result.k = k;

    double *distanceMatrix = calculateDistance(query, corpus, m, n, d);

    double kth;
    int index;
    double *distanceCopy = new double[n];
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < k; j++)
        {
            std::copy(distanceMatrix + i * n, distanceMatrix + (i + 1) * n, distanceCopy);
            kth = kthSmallest(distanceCopy, 0, n - 1, j + 1);
            auto it = std::find(distanceMatrix + i * n, distanceMatrix + (i + 1) * n, kth);

            if (kth == result.ndist[i * k + j - 1] && j != 0)
            {
                it = std::find(distanceMatrix + i * n + result.nidx[i * k + j - 1] + 1, distanceMatrix + (i + 1) * n, kth);
            }

            result.nidx[i * k + j] = (it - distanceMatrix + i * n) % n;
            result.ndist[i * k + j] = kth;
        }
    }

    delete[] distanceMatrix;
    delete[] distanceCopy;

    return result;
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
        indexConverter = (selfTID - i + numTasks) % numTasks;
        compareResults(&result, &tempResult, n, k, indexConverter, averageSize);
    }

    delete[] corpus;
    delete[] incoming;

    return result;
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
                result->nidx[(i + 1) * k - 1] = temp->nidx[i * k + j] + index * coefficient;
                result->ndist[(i + 1) * k - 1] = temp->ndist[i * k + j];
                quickSort(&result->ndist[i * k], 0, k - 1, &result->nidx[i * k]);
            }
        }
    }
}
