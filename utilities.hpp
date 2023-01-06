#include <algorithm>
#include <numeric>
#include <cmath>
#include <gsl/gsl_cblas.h>

/**
 * Calculates A.^2
 * 
 * @param A the array to calculate A.^2
 * @param nrows the amount of rows of the array
 * @param ncols the amount of columns of the array
 * @return pointer to A.^2
 */
double* squared(double *A, int nrows, int ncols)
{
    double* result = new double[nrows * ncols];

    std::copy(A, A + nrows * ncols, result);
    std::transform(result, result + nrows * ncols, result, [](double x) { return x * x; });

    return result;
}

/**
 * Calculates sum(A, 2)
 * 
 * @param A the array to calculate sum(A, 2)
 * @param nrows the amount of rows of the array
 * @param ncols the amount of columns of the array
 * @return pointer to sum(A, 2) 
 */
double* rowSum(double *A, int nrows, int ncols)
{
    double* result = new double[nrows];

    for (int i = 0; i < nrows; i++)
        result[i] = std::accumulate(A + i * ncols, A + (i + 1) * ncols, 0.0);

    return result;
}

/**
 * Calculates the euclidean distance matrix of two sets of points
 * 
 * @param query the first set of points in row major
 * @param corpus the second set of points in row major
 * @param m the amount of points in the first set
 * @param n the amount of points in the second set
 * @param d the dimensions of the points
 * @return pointer to the euclidean distance matrix
 */
double* calculateDistance(double *query, double *corpus, int m, int n, int d)
{
    double* result = new double[m * n];

    // sum(X.^2, 2)
    double* querySummed = rowSum(squared(query, m, d), m, d);

    // X * Y.'
    double* queryCorpus = new double[m * n];
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans, m, n, d, 1.0, query, d, corpus, d, 0.0, queryCorpus, n);

    // sum(Y.^2, 2)
    double* corpusSummed = rowSum(squared(corpus, n, d), n, d);

    // no need to transpose Y, it's always 1 column, so just use j index instead of i
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            result[i * n + j] = sqrt(querySummed[i] - 2 * queryCorpus[i * n + j] + corpusSummed[j]);

    delete[] querySummed;
    delete[] queryCorpus;
    delete[] corpusSummed;

    return result;
}
