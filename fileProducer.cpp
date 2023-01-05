#include <iostream>
#include <fstream>
#include <random>

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        std::cout << "Usage: ./fileProducer <filename> <noPoints> <noDimensions>" << std::endl;
        return 1;
    }

    std::ofstream file(argv[1]);
    if (!file)
    {
        std::cout << "Error in opening of the file" << std::endl;
        return 1;
    }

    int noPoints = atoi(argv[2]);
    int noDimensions = atoi(argv[3]);
    file << noPoints << " " << noDimensions << std::endl;

    std::uniform_real_distribution<double> unif(-1000, 1000);
    std::default_random_engine re;
    for (int i = 0; i < noPoints; i++)
    {
        for (int j = 0; j < noDimensions; j++)
            file << unif(re) << " ";
        file << std::endl;
    }

    return 0;
}