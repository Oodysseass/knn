#include <algorithm>
#include <climits>

#include <iomanip>

#include "knnresult.hpp"
#include "matrices.hpp"

int partition(double arr[], int l, int r)
{
    double x = arr[r];
    int i = l;
    for (int j = l; j <= r - 1; j++)
    {
        if (arr[j] <= x)
        {
            std::swap(arr[i], arr[j]);
            i++;
        }
    }
    std::swap(arr[i], arr[r]);
    return i;
}

double kthSmallest(double arr[], int l, int r, int k)
{
    if (k > 0 && k <= r - l + 1)
    {

        int index = partition(arr, l, r);

        if (index - l == k - 1)
            return arr[index];

        if (index - l > k - 1)
            return kthSmallest(arr, l, index - 1, k);

        return kthSmallest(arr, index + 1, r, k - index + l - 1);
    }

    return INT_MAX;
}

Knnresult kNN(double *query, double *corpus, int m, int n, int d, int k)
{
    Knnresult result = Knnresult(m, k);
    result.nidx = new int[m * k];
    result.ndist = new double[m * k];
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

            if (kth == result.ndist[i * k + j - 1])
                it = std::find(distanceMatrix + i * n + result.nidx[i * k + j - 1] + 1, distanceMatrix + (i + 1) * n, kth);

            result.nidx[i * k + j] = (it - distanceMatrix + i * n) % n;
            result.ndist[i * k + j] = kth;
        }
    }

/*     for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
            std::cout << std::setfill(' ') << std::setw(8) << std::fixed << std::setprecision(2) << distanceMatrix[i * n + j] << " ";
        std::cout << std::endl;
    } */

    return result;
}