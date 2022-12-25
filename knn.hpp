#include <iostream>
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


Knnresult* kNN(double *query, double *corpus, int m, int n, int d, int k)
{
    Knnresult* result = new Knnresult;
    result->nidx = new int[m * k];
    result->ndist = new double[m * k];
    result->m = m;
    result->k = k;

    double* distanceMatrix = calculateDistance(query, corpus, m, n, d);

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
            std::cout << distanceMatrix[i * m + j] << " ";
        std::cout << std::endl;
    }

    return result;
}

// implements this matlab code: D = sqrt(sum(X.^2, 2) - 2 * X * Y.' + sum(Y.^2, 2).')
double* calculateDistance(double *query, double *corpus, int m, int n, int d)
{
    double* result = new double[m * n];

    // X.^2
    double* querySquared = new double[m * d];
    std::copy(query, query + m * d, querySquared);
    std::transform(querySquared, querySquared + m * d, querySquared, [](double x) { return x * x; });

    // sum(X.^2, 2)
    double* querySummed = new double[m];
    for (int i = 0; i < m; i++)
        querySummed[i] = std::accumulate(querySquared + i * d, querySquared + (i + 1) * d, 0.0);

    // X * Y.'
    double* queryCorpus = new double[m * n];
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans, m, n, d, 1.0, query, d, corpus, d, 0.0, queryCorpus, n);

    // Y.^2
    double* corpusSquared = new double[n * d];
    std::copy(corpus, corpus + n * d, corpusSquared);
    std::transform(corpusSquared, corpusSquared + n * d, corpusSquared, [](double x) { return x * x; });

    // sum(Y.^2, 2)
    double* corpusSummed = new double[n];
    for (int i = 0; i < n; i++)
        corpusSummed[i] = std::accumulate(corpusSquared+ i * d, corpusSquared + (i + 1) * d, 0.0);

    // no need to transpose Y, it's always 1 column, so just use j index instead of i
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            result[i * n + j] = sqrt(querySummed[i] - 2 * queryCorpus[i * n + j] + corpusSummed[j]);

    return result;
}