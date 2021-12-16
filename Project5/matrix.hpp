#pragma once
#pragma GCC optimize(3, "Ofast", "inline")
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>

using namespace std;

template <class T>
class Matrix
{
public:
    size_t rows;
    size_t cols;
    int channel;
    T *data;
    int *refcount;

    Matrix();

    Matrix(const Matrix &m);

    Matrix(size_t rows, size_t cols, int channel = 1);

    Matrix(T *data, size_t rows, size_t cols, int channel = 1);
    T &operator()(size_t x, size_t y, int z = 0);
    void resize(size_t rows, size_t cols, int channel = 1);
    const Matrix &operator=(const Matrix &b);

    const Matrix &operator=(T b);

    Matrix operator+(const Matrix &b) const;

    Matrix operator+(T b) const;

    friend Matrix operator+(const T a, const Matrix &b)
    {
        return b + a;
    }

    Matrix operator-(const Matrix &b) const;

    Matrix operator-(T b) const;

    friend Matrix operator-(const T a, const Matrix &b)
    {
        if (b.data == NULL)
        {
            cerr << "The matrix is empty!" << endl;
            return Matrix();
        }
        Matrix result(b.rows, b.cols, b.channel);
        T *ptr = result.data;
        T *ptrb = b.data;
        int total = b.channel * b.rows * b.cols;
        while (total--)
            *(ptr++) = a - *(ptrb++);
        return result;
    }

    Matrix operator*(const Matrix &b) const;

    Matrix operator*(T b) const;

    friend Matrix operator*(const T a, const Matrix &b)
    {
        return b * a;
    }

    bool operator==(const Matrix &b) const;

    bool operator==(T b) const;

    friend bool operator==(const T a, const Matrix &b)
    {
        return b == a;
    }

    Matrix clone() const;

    friend ostream &operator<<(ostream &os, const Matrix &matrix)
    {
        T *ptrs[matrix.channel];
        for (int counter = 0; counter < matrix.channel; ++counter)
            ptrs[counter] = matrix.data + matrix.rows * matrix.cols * counter;
        os << "[";
        for (int i = 1; i <= matrix.rows; ++i)
        {
            if (i != 1)
                os << " ";
            for (int j = 1; j <= matrix.cols; ++j)
            {
                for (int k = 1; k <= matrix.channel; ++k)
                {
                    if (j == matrix.cols && k == matrix.channel)
                    {
                        if (i != matrix.rows)
                            os << *(ptrs[k - 1]++) << ";" << endl;
                        else
                            os << *(ptrs[k - 1]++);
                    }
                    else
                    {
                        if (k == matrix.channel)
                            os << *(ptrs[k - 1]++) << ", ";
                        else
                            os << *(ptrs[k - 1]++) << " ";
                    }
                }
            }
        }
        os << "]" << endl;
        return os;
    }

    void release();

    ~Matrix();
};

template class Matrix<unsigned char>;
template class Matrix<short>;
template class Matrix<int>;
template class Matrix<float>;
template class Matrix<double>;