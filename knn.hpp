#include "knnresult.hpp"
#include "matrices.hpp"
#include <iomanip>

Knnresult* kNN(double *query, double *corpus, int m, int n, int d, int k);

double* calculateDistance(double *query, double *corpus, int m, int n, int d);


Knnresult* kNN(double *query, double *corpus, int m, int n, int d, int k)
{
    Knnresult* result = new Knnresult(m, k);
    result->nidx = new int[m * k];
    result->ndist = new double[m * k];
    result->m = m;
    result->k = k;

    double* distanceMatrix = calculateDistance(query, corpus, m, n, d);

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
            std::cout << std::fixed << std::setprecision(4) << distanceMatrix[i * n + j] << " ";
        std::cout << std::endl;
    }

    return result;
}