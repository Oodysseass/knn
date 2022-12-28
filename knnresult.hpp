struct  Knnresult
{
    int *nidx;      //ids of knn
    double *ndist;  //distance of knn
    int m;          //number of corpus points
    int k;          //number of neighbors

    Knnresult(int m, int k)
    {
        nidx = new int[m * k];
        ndist = new double[m * k];
        this->m = m;
        this->k = k;
    }
};

