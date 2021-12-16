#include "matrix.hpp"

template <class T>
Matrix<T>::Matrix()
{
    rows = cols = channel = 0;
    data = NULL;
    refcount = new int[1];
    refcount[0] = 1;
}

template <class T>
Matrix<T>::Matrix(size_t rows, size_t cols, int channel)
{
    if (rows == 0 || cols == 0 || channel == 0)
    {
        rows = cols = channel = 0;
        data = NULL;
        cerr << "Rows and cols must greater than 0." << endl;
    }
    else
    {
        this->rows = rows;
        this->cols = cols;
        this->channel = channel;
        data = new T[rows * cols * channel]{};
    }
    refcount = new int[1];
    refcount[0] = 1;
}
template <class T>
Matrix<T>::Matrix(T *data, size_t rows, size_t cols, int channel)
{
    refcount = new int[1];
    if (rows == 0 || cols == 0 || channel == 0)
    {
        rows = cols = channel = 0;
        data = NULL;
        cerr << "Rows and cols must greater than 0." << endl;
        refcount[0] = 1;
    }
    else
    {
        this->rows = rows;
        this->cols = cols;
        this->channel = channel;
        this->data = data;
        refcount[0] = 2;
    }
}

template <class T>
void Matrix<T>::resize(size_t rows, size_t cols, int channel)
{
    if (this->rows * this->cols * this->channel != rows * cols * channel)
        cerr << "Resize failed!" << endl;
    else
    {
        this->rows = rows;
        this->cols = cols;
        this->channel = channel;
    }
}
template <class T>
Matrix<T>::Matrix(const Matrix<T> &m)
{
    rows = m.rows;
    cols = m.cols;
    data = m.data;
    channel = m.channel;
    refcount = m.refcount;
    ++*refcount;
}

template <class T>
void Matrix<T>::release()
{
    --*refcount;
    if (*refcount == 0)
    {
        if (data != NULL)
        {
            delete[] data;
        }
        delete[] refcount;
    }
    rows = cols = channel = 0;
    data = NULL;
    refcount = NULL;
}

template <class T>
Matrix<T>::~Matrix()
{
    release();
}

template <class T>
T &Matrix<T>::operator()(size_t x, size_t y, int z)
{
    if (x >= rows || y >= cols || z >= channel)
        return *data;
    return *(data + x * cols + y + cols * rows * z);
}

template <class T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> &b) const
{
    if (rows != b.rows || cols != b.cols || channel != b.channel)
    {
        cerr << "Two matrixes aren't same size!" << endl;
        return Matrix();
    }
    if (data == NULL || b.data == NULL)
    {
        cerr << "The matrix is empty!" << endl;
        return Matrix();
    }
    Matrix<T> result(rows, cols, channel);
    T *ptra = data;
    T *ptrb = b.data;
    T *ptr = result.data;
    size_t total = channel * rows * cols;
    while (total--)
        *(ptr++) = *(ptra++) + *(ptrb++);
    return result;
}

template <class T>
Matrix<T> Matrix<T>::operator+(const T b) const
{
    if (data == NULL)
    {
        cerr << "The matrix is empty!" << endl;
        return Matrix();
    }
    Matrix result(rows, cols, channel);
    T *ptr = result.data;
    T *ptra = data;
    size_t total = channel * rows * cols;
    while (total--)
        *(ptr++) = *(ptra++) + b;
    return result;
}

template <class T>
Matrix<T> Matrix<T>::operator-(const Matrix<T> &b) const
{
    if (rows != b.rows || cols != b.cols || channel != b.channel)
    {
        cerr << "Two matrixes aren't same size!" << endl;
        return Matrix();
    }
    if (data == NULL || b.data == NULL)
    {
        cerr << "The matrix is empty!" << endl;
        return Matrix();
    }
    Matrix<T> result(rows, cols, channel);
    T *ptra = data;
    T *ptrb = b.data;
    T *ptr = result.data;
    size_t total = channel * rows * cols;
    while (total--)
        *(ptr++) = *(ptra++) - *(ptrb++);
    return result;
}

template <class T>
Matrix<T> Matrix<T>::operator-(const T b) const
{
    if (data == NULL)
    {
        cerr << "The matrix is empty!" << endl;
        return Matrix();
    }
    Matrix result(rows, cols, channel);
    T *ptr = result.data;
    T *ptra = data;
    size_t total = channel * rows * cols;
    while (total--)
        *(ptr++) = *(ptra++) - b;
    return result;
}

template <class T>
Matrix<T> Matrix<T>::operator*(const Matrix<T> &b) const
{
    if (cols != b.rows || channel != b.channel)
    {
        cerr << "Can not multiple these two matrixes." << endl;
        return Matrix();
    }
    if (data == NULL || b.data == NULL)
    {
        cerr << "The matrix is empty!" << endl;
        return Matrix();
    }
    Matrix<T> result(rows, b.cols, channel);
    T *ptr;
    T *ptra;
    T *ptrb;
    for (size_t count = 0; count < channel; ++count)
    {
        ptr = result.data + result.rows * result.cols * count;
        ptra = data + rows * cols * count;
        ptrb = b.data + b.rows * b.cols * count;
        for (size_t i = 0; i < rows; ++i)
        {
            for (size_t k = 0; k < cols; ++k)
            {
                for (size_t j = 0; j < b.cols; ++j)
                    *(ptr++) += *ptra * *(ptrb++);
                ptr -= result.cols;
                ++ptra;
            }
            ptr += result.cols;
            ptrb -= b.rows * b.cols;
        }
    }
    return result;

}

template <class T>
Matrix<T> Matrix<T>::operator*(const T b) const
{
    if (data == NULL)
    {
        cerr << "The matrix is empty!" << endl;
        return Matrix();
    }
    Matrix result(rows, cols, channel);
    T *ptr = result.data;
    T *ptra = data;
    size_t total = channel * rows * cols;
    while (total--)
        *(ptr++) = *(ptra++) * b;
    return result;
}

template <class T>
bool Matrix<T>::operator==(const Matrix<T> &b) const
{
    if (rows != b.rows || cols != b.cols || channel != b.channel)
        return false;
    T *ptra = data;
    T *ptrb = b.data;
    size_t total = channel * rows * cols;
    while (total--)
        if (*(ptra++) != *(ptrb++))
            return false;
    return true;
}

template <class T>
bool Matrix<T>::operator==(const T b) const
{
    T *ptr = data;
    size_t total = channel * rows * cols;
    while (total--)
        if (*(ptr++) != b)
            return false;
    return true;
}

template <class T>
const Matrix<T> &Matrix<T>::operator=(const Matrix<T> &b)
{
    release();
    rows = b.rows;
    cols = b.cols;
    data = b.data;
    channel = b.channel;
    refcount = b.refcount;
    ++*refcount;
    return *this;
}

template <class T>
const Matrix<T> &Matrix<T>::operator=(const T b)
{
    if (data == NULL)
        return *this;
    T *ptr = data;
    size_t total = channel * rows * cols;
    while (total--)
        *(ptr++) = b;
    return *this;
}

template <class T>
Matrix<T> Matrix<T>::clone() const
{
    Matrix newMat(rows, cols, channel);
    size_t total = channel * rows * cols;
    memcpy(newMat.data, data, total);
    return newMat;
}