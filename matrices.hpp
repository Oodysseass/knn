#include <algorithm>
#include <numeric>
#include <cmath>
#include <gsl/gsl_cblas.h>

double* calculateDistance(double *query, double *corpus, int m, int n, int d);

double* squared(double *A, int nrows, int ncols);

double* rowSum(double *A, int nrows, int ncols);

// Calculates euclidean distance matrix by
// implementing this matlab code: 
// D = sqrt(sum(X.^2, 2) - 2 * X * Y.' + sum(Y.^2, 2).')
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

    return result;
}

// returns pointer to A.^2
double* squared(double *A, int nrows, int ncols)
{
    double* result = new double[nrows * ncols];

    std::copy(A, A + nrows * ncols, result);
    std::transform(result, result + nrows * ncols, result, [](double x) { return x * x; });

    return result;
}

// return pointer to sum(A, 2)
double* rowSum(double *A, int nrows, int ncols)
{
    double* result = new double[nrows];

    for (int i = 0; i < nrows; i++)
        result[i] = std::accumulate(A + i * ncols, A + (i + 1) * ncols, 0.0);

    return result;
}