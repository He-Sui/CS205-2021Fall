#pragma GCC optimize(3, "Ofast", "inline")
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <ctime>
#include <iomanip>

using namespace std;
int matrixSize(string);
void matrixMultiplication(double **, double **, double **, int);

int main(int argc, char **argv)
{
    string inFileName1, inFileName2, outFileName;
    int size;
    clock_t startTime, endTime;
    if (argc == 4)
    {
        inFileName1 = argv[1];
        inFileName2 = argv[2];
        outFileName = argv[3];
    }
    else
    {
        cerr << "Missing file!" << endl;
        return -1;
    }
    ifstream inFile1(inFileName1);
    ifstream inFile2(inFileName2);
    if (!inFile1.is_open() || !inFile2.is_open())
    {
        cerr << "Can not open the file!" << endl;
        return -1;
    }
    int size1 = matrixSize(inFileName1);
    int size2 = matrixSize(inFileName2);
    if (size1 != size2)
    {
        cerr << "Two matrix are not the same size!" << endl;
        return -1;
    }

    size = size1;
    double **matrixA = new double *[size];
    double **matrixB = new double *[size];
    double **result = new double *[size];

    for (int i = 0; i < size; ++i)
    {
        matrixA[i] = new double[size];
        matrixB[i] = new double[size];
        result[i] = new double[size];
    }

    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
        {
            inFile1 >> matrixA[i][j];
            inFile2 >> matrixB[i][j];
        }
    cout << "Load successfully!" << endl;
    inFile1.close();
    inFile2.close();

    startTime = clock();
    matrixMultiplication(matrixA, matrixB, result, size);
    endTime = clock();
    cout << "The calculate time is: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;

    ofstream outFile(outFileName);
    // outFile << setprecision(20);

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            outFile << result[i][j] << " ";
            if (result[i][j] <= 0)
                cout << i << " " << j << endl;
        }
        outFile << endl;
    }
    outFile.close();
    cout << "Write successfully!" << endl;
    return 0;
}

int matrixSize(string name)
{
    int startIndex = -1;
    int endIndex = -1;
    for (int counter = 0; counter < name.size(); ++counter)
    {
        if (name[counter] >= 48 && name[counter] <= 57 && startIndex == -1)
            startIndex = counter;
        else if ((name[counter] < 48 || name[counter] > 57) && startIndex != -1)
        {
            endIndex = counter - 1;
            break;
        }
    }
    int size = 0;
    for (int counter = startIndex; counter <= endIndex; counter++)
        size += (name[counter] - 48) * (int)pow(10, endIndex - counter);
    return size;
}

// Version 1
// void matrixMultiplication(double **matrixA, double **matrixB, double **result, int size)
// {
//     for (int i = 0; i < size; ++i)
//         for (int j = 0; j < size; ++j)
//             for (int k = 0; k < size; ++k)
//                 result[i][j] += matrixA[i][k] * matrixB[k][j];
// }

// Version 2
// void matrixMultiplication(double **matrixA, double **matrixBT, double **result, int size)
// {
//     for (int i = 0; i < size; i++)
//         for (int j = 0; j < size; j++)
//             for (int k = 0; k < size; k++)
//                 result[i][j] += matrixA[i][k] * matrixBT[j][k];
// }

// Version 3
void matrixMultiplication(double **matrixA, double **matrixB, double **result, int size)
{
    for (int i = 0; i < size; ++i)
        for (int k = 0; k < size; ++k)
        {
            double value = matrixA[i][k];
            for (int j = 0; j < size; ++j)
                result[i][j] += value * matrixB[k][j];
        }
}