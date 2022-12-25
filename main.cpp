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

    double* distanceMatrix = calculateDistance(query, corpus, m, n, d);

    return result;
}

double* calculateDistance(double *query, double *corpus, int m, int n, int d)
{
    double* result = new double[m * n];

    double* querySquared = new double[m * d];
    std::copy(query, query + m * d, querySquared);
    std::transform(querySquared, querySquared + m * d, querySquared, [](double x) { return x * x; });

    double* querySummed = new double[m];
    for (int i = 0; i < m; i++)
        querySummed[i] = std::accumulate(querySquared + i * d, querySquared + (i + 1) * d, 0.0);

    double* queryCorpus = new double[m * n];
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans, m, n, d, 1.0, query, d, corpus, d, 0.0, queryCorpus, n);

    double* corpusSquared = new double[n * d];
    std::copy(corpus, corpus + n * d, corpusSquared);
    std::transform(corpusSquared, corpusSquared + n * d, corpusSquared, [](double x) { return x * x; });

    double* corpusSummed = new double[n];
    for (int i = 0; i < n; i++)
        corpusSummed[i] = std::accumulate(corpusSquared+ i * d, corpusSquared + (i + 1) * d, 0.0);

    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            result[i * n + j] = sqrt(querySummed[i] - 2 * queryCorpus[i * n + j] + corpusSummed[j]);

    return result;
}