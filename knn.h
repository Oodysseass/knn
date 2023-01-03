#ifndef KNN_H
#define KKN_H

#include <algorithm>
#include <climits>
#include <mpi.h>

/**
 * Struct denoting the results of KNN algorithm
 */
struct  Knnresult
{
    int *nidx;      //ids of knn
    double *ndist;  //distance of knn
    int m;          //number of corpus points
    int k;          //number of neighbors

    /**
     * Knnresult Constructor
     * 
     * @param m the amount of points whose neighbors we want
     * @param k the amount of neighbors we want to find
     */
    Knnresult(int m, int k)
    {
        nidx = new int[m * k];
        ndist = new double[m * k];
        this->m = m;
        this->k = k;
    }
};

/**
 * Find the kth lowest value of an array
 * 
 * @param A array whose kth value we want
 * @param low index on the left end of the array
 * @param high index on the right end of the array
 * @param k the order of the value we want to find
 * @return the value of the kth lowest value
 */
double kthSmallest(double A[], int low, int high, int k);

/**
 * Places pivot in array
 * 
 * @param A the array on which we are working to place the pivot
 * @param low index on the left and of the array
 * @param high index on the right end of the array, A[high] starting value of pivot
 * @return the index of the pivot at its correct possition
 */
int partition(double A[], int low, int high);

/**
 * Finds the k-nearest neighbors of points
 * 
 * @param query the set of points whose neighbors we will find
 * @param corpus the set of points we are looking for neighbors
 * @param m the amount of points in query
 * @param n the amount of points in n
 * @param d the dimensions of the points
 * @param k the amount of neighbors we want to find
 * @return a struct with the indexes and the distances of the k-nearest neighbors
 */
Knnresult kNN(double *query, double *corpus, int m, int n, int d, int k);

/**
 * Implements knn with MPI
 * 
 * @param query the set of points that each process will keep
 * @param n the amount of points in query
 * @param totalPoints the sum of all points each process has
 * @param d the dimensions of the points
 * @param k the amount of neighbors we want to find
 */
Knnresult distrallkNN(double *query, int n, int totalPoints, int d, int k);

/**
 * Compares two results of KNN to keep the actual nearest neighbors.
 * Used in distrallkNN.
 * 
 * @param result a pointer to the result of KNN we currently have
 * @param temp a pointer to the result of KNN we want to check
 * @param n the amount of points
 * @param k the amount of nearest neighbors
 * @param index used to find which process gave us the new set of points we are comparing
 * @param coefficient used with index to find the global ID of the new points
 */
void compareResults(Knnresult *result, Knnresult *temp, int n, int k, int index, int coefficient);

#endif