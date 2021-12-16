#include "matrix.hpp"

template <class T>
Matrix<T>::Matrix()
{
    rows = cols = step = channel = 0;
    data = data_start = data_end = NULL;
    refcount = new int[1];
    refcount[0] = 1;
    isSubMatrix = false;
}

template <class T>
Matrix<T>::Matrix(int rows, int cols, int channel)
{
    if (rows <= 0 || cols <= 0 || channel <= 0)
    {
        rows = cols = step = channel = 0;
        data = data_start = data_end = NULL;
        cerr << "Rows and cols must greater than 0." << endl;
    }
    else
    {
        this->rows = rows;
        this->cols = cols;
        this->channel = channel;
        data = new T[rows * cols * channel]();
        step = cols;
        data_start = data;
        data_end = data + rows * cols * channel;
    }
    refcount = new int[1];
    refcount[0] = 1;
    isSubMatrix = false;
}

template <class T>
Matrix<T>::Matrix(const Matrix<T> &m)
{
    rows = m.rows;
    cols = m.cols;
    data = m.data;
    channel = m.channel;
    step = m.step;
    data_start = m.data_start;
    data_end = m.data_end;
    refcount = m.refcount;
    ++*refcount;
    isSubMatrix = m.isSubMatrix;
}

template <class T>
Matrix<T>::Matrix(const Matrix<T> &b, int x, int y, int rows, int cols)
{
    if (x <= 0 || y <= 0 || rows <= 0 || cols <= 0 || x + rows - 1 > b.rows || y + cols - 1 > b.cols)
    {
        cerr << "Cannot generate the matrix." << endl;
        new (this) Matrix();
    }
    else
    {
        this->rows = rows;
        this->cols = cols;
        channel = b.channel;
        data = b.data + ((x - 1) * b.step + y - 1) * channel;
        step = b.step;
        data_start = b.data_start;
        data_end = b.data_end;
        refcount = b.refcount;
        ++*refcount;
        if (rows == b.rows && cols == b.cols)
            isSubMatrix = false;
        else
            isSubMatrix = true;
    }
}

template <class T>
void Matrix<T>::release()
{
    --*refcount;
    if (*refcount == 0)
    {
        if (data_start != NULL)
        {
            delete[] data_start;
            // cout << "Delete data!" << endl;
        }
        delete[] refcount;
    }
    rows = cols = step = channel = 0;
    data = data_start = data_end = NULL;
    refcount = NULL;
}

template <class T>
Matrix<T>::~Matrix()
{
    // cout << "Delete Matrix!" << endl;
    release();
}

template <class T>
T &Matrix<T>::at(int x, int y, int z)
{
    if (x < 1 || x > rows || y < 1 || y > cols || z < 1 || z > channel)
        return *data;
    return *(data + ((x - 1) * step + y - 1) * channel + z - 1);
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
    if (!isSubMatrix && !b.isSubMatrix)
    {
        for (int i = 0; i < rows * cols * channel; ++i)
            result.data[i] = data[i] + b.data[i];
        return result;
    }
    T *ptra = data;
    T *ptrb = b.data;
    T *ptr = result.data;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols * channel; ++j)
        {
            *ptr = *ptra + *ptrb;
            ++ptr;
            ++ptra;
            ++ptrb;
        }
        ptra = ptra + (step - cols) * channel;
        ptrb = ptrb + (b.step - b.cols) * b.channel;
    }
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
    if (!isSubMatrix)
    {
        for (int i = 0; i < rows * cols * channel; ++i)
            result.data[i] = data[i] + b;
        return result;
    }
    T *ptr = result.data;
    T *ptra = data;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols * channel; ++j)
        {
            *ptr = *ptra + b;
            ++ptr;
            ++ptra;
        }
        ptra += (step - cols) * channel;
    }
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
    if (!isSubMatrix && !b.isSubMatrix)
    {
        for (int i = 0; i < rows * cols * channel; ++i)
            result.data[i] = data[i] - b.data[i];
        return result;
    }
    T *ptra = data;
    T *ptrb = b.data;
    T *ptr = result.data;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols * channel; ++j)
        {
            *ptr = *ptra - *ptrb;
            ++ptr;
            ++ptra;
            ++ptrb;
        }
        ptra += (step - cols) * channel;
        ptrb += (b.step - b.cols) * b.channel;
    }
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
    if (!isSubMatrix)
    {
        for (int i = 0; i < rows * cols * channel; ++i)
            result.data[i] = data[i] - b;
        return result;
    }
    T *ptr = result.data;
    T *ptra = data;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols * channel; ++j)
        {
            *ptr = *ptra - b;
            ++ptr;
            ++ptra;
        }
        ptra += (step - cols) * channel;
    }
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
    for (int count = 0; count < channel; ++count)
    {
        ptr = result.data + count;
        ptra = data + count;
        ptrb = b.data + count;
        for (int i = 0; i < rows; ++i)
        {
            for (int k = 0; k < cols; ++k)
            {
                for (int j = 0; j < b.cols; ++j)
                {
                    *ptr += *ptrb * *ptra;
                    ptr += result.channel;
                    ptrb += b.channel;
                }
                ptr -= result.cols * result.channel;
                ptra += channel;
                ptrb += (b.step - b.cols) * b.channel;
            }
            ptra += (step - cols) * channel;
            ptrb = b.data + count;
            ptr += result.step * result.channel;
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
    if (!isSubMatrix)
    {
        for (int i = 0; i < rows * cols * channel; ++i)
            result.data[i] = data[i] * b;
        return result;
    }
    T *ptr = result.data;
    T *ptra = data;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            for (int k = 0; k < channel; ++k)
            {
                *ptr = *ptra * b;
                ++ptr;
                ++ptra;
            }
        }
        ptra += (step - cols) * channel;
    }
    return result;
}

template <class T>
bool Matrix<T>::operator==(const Matrix<T> &b) const
{
    if (rows != b.rows || cols != b.cols || channel != b.channel)
        return false;
    if (!isSubMatrix && !b.isSubMatrix)
    {
        for (int i = 0; i < rows * cols * channel; ++i)
        {
            if (data[i] != b.data[i])
                return false;
        }
        return true;
    }
    T *ptra = data;
    T *ptrb = b.data;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols * channel; ++j)
        {
            if (*ptra != *ptrb)
                return false;
            ++ptra;
            ++ptrb;
        }
        ptra += (step - cols) * channel;
        ptrb += (b.step - b.cols) * b.channel;
    }
    return true;
}

template <class T>
bool Matrix<T>::operator==(const T b) const
{
    if (!isSubMatrix)
    {
        for (int i = 0; i < rows * cols * channel; ++i)
            if (data[i] != b)
                return false;
        return true;
    }
    T *ptr = data;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols * channel; ++j)
        {
            if (*ptr != b)
                return false;
            ++ptr;
        }
        ptr += (step - cols) * channel;
    }
    return true;
}

template <class T>
bool Matrix<T>::operator!=(const Matrix<T> &b) const
{
    if (rows != b.rows || cols != b.cols || channel != b.channel)
        return true;
    if (!isSubMatrix && !b.isSubMatrix)
    {
        for (int i = 0; i < rows * cols * channel; ++i)
        {
            if (data[i] != b.data[i])
                return true;
        }
        return false;
    }
    T *ptra = data;
    T *ptrb = b.data;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols * channel; ++j)
        {
            if (*ptra != *ptrb)
                return true;
            ++ptra;
            ++ptrb;
        }
        ptra += (step - cols) * channel;
        ptrb += (b.step - b.cols) * b.channel;
    }
    return false;
}

template <class T>
bool Matrix<T>::operator!=(const T b) const
{
    if (!isSubMatrix)
    {
        for (int i = 0; i < rows * cols * channel; ++i)
            if (data[i] != b)
                return true;
        return false;
    }
    T *ptr = data;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols * channel; ++j)
        {
            if (*ptr != b)
                return true;
            ++ptr;
        }
        ptr += (step - cols) * channel;
    }
    return false;
}

template <class T>
const Matrix<T> &Matrix<T>::operator=(const Matrix<T> &b)
{
    release();
    rows = b.rows;
    cols = b.cols;
    data = b.data;
    channel = b.channel;
    step = b.step;
    data_start = b.data_start;
    data_end = b.data_end;
    refcount = b.refcount;
    ++*refcount;
    isSubMatrix = b.isSubMatrix;
    return *this;
}

template <class T>
const Matrix<T> &Matrix<T>::operator=(const T b)
{
    T *ptr = data;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols * channel; ++j)
        {
            *ptr = b;
            ++ptr;
        }
        ptr += (step - cols) * channel;
    }
    return *this;
}

template <class T>
Matrix<T> Matrix<T>::subMatrix(int x, int y, int row, int col) const
{
    return Matrix(*this, x, y, row, col);
}

template <class T>
Matrix<T> &Matrix<T>::adjustROI(int dtop, int dbottom, int dleft, int dright)
{
    if (data == NULL)
        return *this;
    //locate the submatrix first
    int element = (data - data_start) / channel;
    int x = element / step + 1;
    int y = element + 1 - (x - 1) * step;
    int row = (data_end - data_start + 1) / channel / step;

    int leftBound, rightBound, upBound, lowBound;
    upBound = x - dtop;
    lowBound = x + rows - 1 + dbottom;
    leftBound = y - dleft;
    rightBound = y + cols - 1 + dright;
    if (lowBound < upBound || leftBound > rightBound)
    {
        cerr << "Can not adjust ROI!" << endl;
        return *this;
    }
    if (upBound <= 0)
        upBound = 1;
    if (lowBound > row)
        lowBound = row;
    if (leftBound <= 0)
        leftBound = 1;
    if (rightBound > step)
        rightBound = step;
    cols = rightBound - leftBound + 1;
    rows = lowBound - upBound + 1;
    data = data_start + ((upBound - 1) * step + leftBound - 1) * channel;
    if (rows == row && cols == step)
        isSubMatrix = false;
    return *this;
}

template <class T>
Matrix<T> Matrix<T>::clone() const
{
    Matrix newMat(rows, cols, channel);
    if (!isSubMatrix)
    {
        memcpy(newMat.data, data, rows * cols * channel * sizeof(T));
        return newMat;
    }
    T *ptrdest = newMat.data;
    T *ptr = data;
    for (int i = 0; i < rows; ++i)
    {
        memcpy(ptrdest, ptr, cols * channel * sizeof(T));
        ptr += step * channel;
        ptrdest += newMat.step * channel;
    }
    return newMat;
}

template <class T>
void Matrix<T>::readFile(string file_path)
{
    ifstream ifs(file_path);
    if (!ifs.is_open())
    {
        cerr << "Can not open the file!" << endl;
        return;
    }
    T *ptr = data;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols * channel; ++j)
        {
            if (!ifs.eof())
            {
                ifs >> *ptr;
                ++ptr;
            }
            else
            {
                ifs.close();
                return;
            }
        }
        ptr += (step - cols) * channel;
    }
    ifs.close();
    cout << "Read successfully!" << endl;
}

template <class T>
int Matrix<T>::getRows() const
{
    return rows;
}
template <class T>
int Matrix<T>::getCols() const
{
    return cols;
}
template <class T>
int Matrix<T>::getChannel() const
{
    return channel;
}
template <class T>
bool Matrix<T>::getIsSubMatrix() const
{
    return isSubMatrix;
}
template <class T>
int Matrix<T>::getReferenceCount() const
{
    return *refcount;
}

template class Matrix<unsigned char>;
template class Matrix<short>;
template class Matrix<int>;
template class Matrix<float>;
template class Matrix<double>;