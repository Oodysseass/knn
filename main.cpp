#include <iostream>
#include <fstream>
#include <string>

#include <algorithm>
#include <numeric>
#include <cmath>
#include <gsl/gsl_cblas.h>


typedef struct {
    int *nidx;
    double *ndist;
    int m;
    int k;
} Knnresult;

Knnresult* kNN(double *query, double *corpus, int m, int n, int d, int k);

double* calculateDistance(double *query, double *corpus, int m, int n, int d);


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


Knnresult* kNN(double *query, double *corpus, int m, int n, int d, int k)
{
    Knnresult* result = new Knnresult;
    result->nidx = new int[m * k];
    result->ndist = new double[m * k];
    result->m = m;
    result->k = k;

    double **querySqr = Hammard(query, query, m, d);
    double *querySum = RowSum(querySqr, m, d);
    double **corpusTrans = Transpose(corpus, m, d);
    double **corpQuerMult = MatrixMultiplication(query, corpusTrans, m, d, d, m);

    return result;
}

double** Hammard(double **A, double **B, int rows, int cols)
{
    double** result = new double*[rows];
    for (int i = 0; i < rows; i++)
        result[i] = new double[cols];

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result[i][j] = A[i][j] * B[i][j];
    
    return result;
}

double* RowSum(double** A, int rows, int cols)
{
    double* result = new double[rows]();

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result[i] += A[i][j];

    return result;
}

double** Transpose(double **A, int rows, int cols)
{
    double** result = new double*[cols];
    for (int i = 0; i < cols; i++)
        result[i] = new double[rows];
    
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result[j][i] = A[i][j];
    
    return result;
}

double** MatrixMultiplication(double **A, double **B, int rowsA, int colsA, int rowsB, int colsB)
{
    if (!(colsA == rowsB))
    {
        cout << "Not valid dimensions for matrix multiplication" << endl;
        return NULL;
    }

    double** result = new double*[rowsA];
    for (int i = 0; i < rowsA; i++)
        result[i] = new double[colsB]();

    for (int i = 0; i < rowsA

    return result;
}