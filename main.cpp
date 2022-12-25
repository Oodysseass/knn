#include <iostream>
#include <fstream>
#include <string>
#include <mpi.h>

using namespace std;

typedef struct {
    int **nidx;
    double **ndist;
    int m;
    int k;
} Knnresult;

Knnresult* kNN(double **query, double **corpus, int m, int n, int d, int k);

double** Hammard(double **A, double **B, int rows, int cols);

double* RowSum(double **A, int rows, int cols);

double** Transpose(double **A, int rows, int cols);

double** MatrixMultiplication(double **A, double **B, int rowsA, int colsA, int rowsB, int colsB);



int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        cout << "Usage: ./main <filename> <numberofneighbors>" << endl;
        return 1;
    }

    ifstream dataFile(argv[1]);
    if (!dataFile)
        cout << "Error in opening of the file" << endl;

    int numNeighbors = atoi(argv[2]);
    int numOfPoints;
    int dimensions;
    dataFile >> numOfPoints >> dimensions;
    
    double** corpusPoints =  new double*[numOfPoints];
    for (int i = 0; i < numOfPoints; i++)
        corpusPoints[i] = new double[dimensions];

    for (int i = 0; i < numOfPoints; i++)
        for (int j = 0; j < dimensions; j++)
            dataFile >> corpusPoints[i][j];

    Knnresult* knnresult = kNN(corpusPoints, corpusPoints, numOfPoints, numOfPoints, dimensions, numNeighbors);

    return 0;
}

Knnresult* kNN(double **query, double **corpus, int m, int n, int d, int k)
{
    Knnresult* result = new Knnresult;
    result->nidx = new int*[m];
    for (int i = 0; i < m; i++)
        result->nidx[i] = new int[k];

    result->ndist = new double*[m];
    for (int i = 0; i < m; i++)
        result->ndist[i] = new double[k];
    
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