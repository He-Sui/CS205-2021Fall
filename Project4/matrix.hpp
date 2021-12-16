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
private:
    int rows;
    int cols;
    int step;
    int channel;
    T *data;
    T *data_start;
    T *data_end;
    bool isSubMatrix;
    int *refcount;

public:
    Matrix();
    Matrix(const Matrix &m);
    Matrix(int rows, int cols, int channel = 1);
    Matrix(const Matrix &b, int x, int y, int row, int col);
    Matrix &adjustROI(int dtop, int dbottom, int dleft, int dright);
    void readFile(string file_path);
    T &at(int x, int y, int z = 1);
    const Matrix &operator=(const Matrix &b);
    const Matrix &operator=(const T b);
    Matrix operator+(const Matrix &b) const;
    Matrix operator+(const T b) const;
    friend Matrix operator+(const T a, const Matrix &b)
    {
        return b + a;
    }

    Matrix operator-(const Matrix &b) const;
    Matrix operator-(const T b) const;
    friend Matrix operator-(const T a, const Matrix &b)
    {
        if (b.data == NULL)
        {
            cerr << "The matrix is empty!" << endl;
            return Matrix();
        }
        Matrix result(b.rows, b.cols, b.channel);
        if (!b.isSubMatrix)
        {
            for (int i = 0; i < b.rows * b.cols * b.channel; ++i)
                result.data[i] = a - b.data[i];
            return result;
        }
        T *ptr = result.data;
        T *ptrb = b.data;
        for (int i = 0; i < b.rows; ++i)
        {
            for (int j = 0; j < b.cols * b.channel; ++j)
            {
                *ptr = a - *ptrb;
                ++ptr;
                ++ptrb;
            }
            ptrb += (b.step - b.cols) * b.channel;
        }
        return result;
    }

    Matrix operator*(const Matrix &b) const;
    Matrix operator*(const T b) const;
    friend Matrix operator*(const T a, const Matrix &b)
    {
        return b * a;
    }

    bool operator==(const Matrix &b) const;
    bool operator==(const T b) const;
    friend bool operator==(const T a, const Matrix &b)
    {
        return b == a;
    }

    bool operator!=(const Matrix &b) const;
    bool operator!=(const T b) const;
    friend bool operator!=(const T a, const Matrix &b)
    {
        return b != a;
    }
    Matrix clone() const;
    int getRows() const;
    int getCols() const;
    int getChannel() const;
    bool getIsSubMatrix() const;
    int getReferenceCount() const;
    Matrix subMatrix(int x, int y, int row, int col) const;
    friend ostream &operator<<(ostream &os, const Matrix &matrix)
    {
        T *ptr = matrix.data;
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
                        {
                            os << *ptr << ";" << endl;
                            ptr += (matrix.step - matrix.cols) * matrix.channel + 1;
                        }
                        else
                            os << *ptr;
                    }
                    else
                    {
                        if (k == matrix.channel)
                            os << *ptr << ", ";
                        else
                            os << *ptr << " ";
                        ++ptr;
                    }
                }
            }
        }
        os << "]" << endl;
        return os;
    }
    friend ofstream &operator<<(ofstream &ofs, const Matrix &matrix)
    {
        T *ptr = matrix.data;
        for (int i = 1; i <= matrix.rows; ++i)
        {
            for (int j = 1; j <= matrix.cols; ++j)
            {
                for (int k = 1; k <= matrix.channel; ++k)
                {
                    if (j == matrix.cols && k == matrix.channel)
                    {
                        if (i != matrix.rows)
                        {
                            ofs << *ptr << ";" << endl;
                            ptr += (matrix.step - matrix.cols) * matrix.channel + 1;
                        }
                        else
                            ofs << *ptr;
                    }
                    else
                    {
                        if (k == matrix.channel)
                            ofs << *ptr << ", ";
                        else
                            ofs << *ptr << " ";
                        ++ptr;
                    }
                }
            }
        }
        return ofs;
    }
    void release();
    ~Matrix();
};

extern template class Matrix<unsigned char>;
extern template class Matrix<short>;
extern template class Matrix<int>;
extern template class Matrix<float>;
extern template class Matrix<double>;