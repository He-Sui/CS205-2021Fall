#include "matrix.h"

struct Matrix *creatMatrix(const int row, const int column, float *const data)
{
    struct Matrix *matrix = (struct Matrix *)malloc(sizeof(struct Matrix));
    matrix->row = row;
    matrix->column = column;
    matrix->data = data;
    return matrix;
};

void deleteMatrix(struct Matrix *matrix)
{
    if (matrix != NULL)
    {
        if (matrix->data != NULL)
            free(matrix->data);
        free(matrix);
    }
}

void copyMatrix(const struct Matrix *const src, struct Matrix *const dest)
{
    dest->row = src->row;
    dest->column = src->column;
    free(dest->data);
    dest->data = (float *)malloc(sizeof(float) * dest->row * dest->column);
    for (int counter = 0; counter < dest->row * dest->column; ++counter)
        dest->data[counter] = src->data[counter];
}

struct Matrix *multiply(const struct Matrix *const a, const struct Matrix *const b)
{
    if (a == NULL || b == NULL || a->column != b->row)
    {
        printf("Can not multiply these two matrixes!\n");
        return NULL;
    }
    struct Matrix *result = creatMatrix(a->row, b->column, (float *)malloc(sizeof(float) * a->row * b->column));
#pragma omp parallel for
    for (int i = 0; i < result->column * result->row; ++i)
        result->data[i] = 0;
#pragma omp parallel for
    for (int i = 1; i <= result->row; ++i)
    {
        for (int k = 1; k <= a->column; ++k)
        {
            float value = a->data[(i - 1) * a->column + k - 1];
            int index_b = (k - 1) * b->column - 1;
            int index_result = (i - 1) * result->column - 1;
            for (int j = 1; j <= result->column; ++j)
                result->data[++index_result] += value * b->data[++index_b];
        }
    }
    return result;
}

void printMatrix(const struct Matrix *const matrix, FILE *file)
{
    for (int i = 0; i < matrix->row * matrix->column; ++i)
    {
        fprintf(file, "%f ", matrix->data[i]);
        if ((i + 1) % matrix->column == 0)
            fprintf(file, "\n");
    }
}

void transpose(struct Matrix *const matrix)
{
    float *data = malloc(sizeof(float) * matrix->column * matrix->row);
    int index = 0;
    for (int i = 0; i < matrix->column * matrix->row; ++i)
    {
        if (i % matrix->row == 0)
            index = i / matrix->row;
        else
            index += matrix->column;
        data[i] = matrix->data[index];
    }
    free(matrix->data);
    matrix->data = data;
    int temp = matrix->row;
    matrix->row = matrix->column;
    matrix->column = temp;
}

struct Matrix *readFile(FILE *file)
{
    float temp;
    char c;
    int row = 0;
    int column = 0;
    int counter = 0;
    char *str;
    while (fscanf(file, "%f", &temp) != EOF)
    {
        ++counter;
        ++column;
        fscanf(file, "%c", &c);
        if (c == '\n' || c == '\r')
            break;
    }
    while (fscanf(file, "%f", &temp) != EOF)
        ++counter;
    if (counter % column != 0)
        return NULL;
    row = counter / column;
    float *data = (float *)malloc(sizeof(float) * row * column);
    rewind(file);
    for (int i = 0; i < row * column; ++i)
        fscanf(file, "%f", &data[i]);
    return creatMatrix(row, column, data);
}

int findIndex(struct Matrix *matrix, int size, int start_index, int part)
{
    int length = size / 2;
    switch (part)
    {
    case 1:
        return start_index;
        break;
    case 2:
        return start_index + length;
    case 3:
        return start_index + matrix->column * length;
    default:
        return start_index + matrix->column * length + length;
    }
}

void *add(int size, struct Matrix *a, int a_start_index, struct Matrix *b, int b_start_index, struct Matrix *result, int result_start_index)
{
    int a_index = a_start_index;
    int b_index = b_start_index;
    int result_index = result_start_index;
    for (int i = 0; i < size * size; ++i)
    {
        result->data[result_index] = a->data[a_index] + b->data[b_index];
        if ((i + 1) % size == 0)
        {
            a_index = a_index + a->column - size;
            b_index = b_index + b->column - size;
            result_index = result_index + result->column - size;
        }
        ++a_index;
        ++b_index;
        ++result_index;
    }
    return result;
}

void *minus(int size, struct Matrix *a, int a_start_index, struct Matrix *b, int b_start_index, struct Matrix *result, int result_start_index)
{
    int a_index = a_start_index;
    int b_index = b_start_index;
    int result_index = result_start_index;
    for (int i = 0; i < size * size; ++i)
    {
        result->data[result_index] = a->data[a_index] - b->data[b_index];
        if ((i + 1) % size == 0)
        {
            a_index = a_index + a->column - size;
            b_index = b_index + b->column - size;
            result_index = result_index + result->column - size;
        }
        ++a_index;
        ++b_index;
        ++result_index;
    }
    return result;
}

void *multiOperations(int size, struct Matrix *a, int a_start_index, struct Matrix *b, int b_start_index, struct Matrix *c, int c_start_index, struct Matrix *d, int d_start_index, struct Matrix *result, int result_start_index, char thirdOperation)
{
    int a_index = a_start_index;
    int b_index = b_start_index;
    int c_index = c_start_index;
    int d_index = d_start_index;
    int result_index = result_start_index;
    for (int i = 0; i < size * size; ++i)
    {
        result->data[result_index] = a->data[a_index] + b->data[b_index] - c->data[c_index];
        switch (thirdOperation)
        {
        case '+':
            result->data[result_index] += d->data[d_index];
            break;
        case '-':
            result->data[result_index] -= d->data[d_index];
            break;
        }
        if ((i + 1) % size == 0)
        {
            a_index = a_index + a->column - size;
            b_index = b_index + b->column - size;
            c_index = c_index + c->column - size;
            d_index = d_index + d->column - size;
            result_index = result_index + result->column - size;
        }
        ++a_index;
        ++b_index;
        ++c_index;
        ++d_index;
        ++result_index;
    }
    return result;
}

struct Matrix *strassen(struct Matrix *a, int a_start_index, struct Matrix *b, int b_start_index, int size)
{
    struct Matrix *result = creatMatrix(size, size, malloc(sizeof(float) * size * size));
    if (size == 128)
    {
        for (int i = 0; i < 16384; ++i)
            result->data[i] = 0;
        for (int i = 1; i <= 128; ++i)
        {
            for (int k = 1; k <= 128; ++k)
            {
                float value = a->data[a_start_index + (i - 1) * a->column + k - 1];
                int index_b = b_start_index + (k - 1) * b->column - 1;
                int index_result = (i - 1) * result->column - 1;
                for (int j = 1; j <= 128; ++j)
                    result->data[++index_result] += value * b->data[++index_b];
            }
        }
        return result;
    }

    int subSize = size / 2;
    int subNum = subSize * subSize;
    struct Matrix *S1 = creatMatrix(subSize, subSize, malloc(sizeof(float) * subNum));
    struct Matrix *S2 = creatMatrix(subSize, subSize, malloc(sizeof(float) * subNum));
    struct Matrix *S3 = creatMatrix(subSize, subSize, malloc(sizeof(float) * subNum));
    struct Matrix *S4 = creatMatrix(subSize, subSize, malloc(sizeof(float) * subNum));
    struct Matrix *S5 = creatMatrix(subSize, subSize, malloc(sizeof(float) * subNum));
    struct Matrix *S6 = creatMatrix(subSize, subSize, malloc(sizeof(float) * subNum));
    struct Matrix *S7 = creatMatrix(subSize, subSize, malloc(sizeof(float) * subNum));
    struct Matrix *S8 = creatMatrix(subSize, subSize, malloc(sizeof(float) * subNum));
    struct Matrix *S9 = creatMatrix(subSize, subSize, malloc(sizeof(float) * subNum));
    struct Matrix *S10 = creatMatrix(subSize, subSize, malloc(sizeof(float) * subNum));
    int a11 = findIndex(a, size, a_start_index, 1);
    int a12 = findIndex(a, size, a_start_index, 2);
    int a21 = findIndex(a, size, a_start_index, 3);
    int a22 = findIndex(a, size, a_start_index, 4);
    int b11 = findIndex(b, size, b_start_index, 1);
    int b12 = findIndex(b, size, b_start_index, 2);
    int b21 = findIndex(b, size, b_start_index, 3);
    int b22 = findIndex(b, size, b_start_index, 4);
    minus(subSize, b, b12, b, b22, S1, 0);
    add(subSize, a, a11, a, a12, S2, 0);
    add(subSize, a, a21, a, a22, S3, 0);
    minus(subSize, b, b21, b, b11, S4, 0);
    add(subSize, a, a11, a, a22, S5, 0);
    add(subSize, b, b11, b, b22, S6, 0);
    minus(subSize, a, a12, a, a22, S7, 0);
    add(subSize, b, b21, b, b22, S8, 0);
    minus(subSize, a, a11, a, a21, S9, 0);
    add(subSize, b, b11, b, b12, S10, 0);

    struct Matrix *P1 = strassen(a, a11, S1, 0, subSize);
    deleteMatrix(S1);
    struct Matrix *P2 = strassen(S2, 0, b, b22, subSize);
    deleteMatrix(S2);
    struct Matrix *P3 = strassen(S3, 0, b, b11, subSize);
    deleteMatrix(S3);
    struct Matrix *P4 = strassen(a, a22, S4, 0, subSize);
    deleteMatrix(S4);
    struct Matrix *P5 = strassen(S5, 0, S6, 0, subSize);
    deleteMatrix(S5);
    deleteMatrix(S6);
    struct Matrix *P6 = strassen(S7, 0, S8, 0, subSize);
    deleteMatrix(S7);
    deleteMatrix(S8);
    struct Matrix *P7 = strassen(S9, 0, S10, 0, subSize);
    deleteMatrix(S9);
    deleteMatrix(S10);
    multiOperations(subSize, P5, 0, P4, 0, P2, 0, P6, 0, result, findIndex(result, size, 0, 1), '+');
    deleteMatrix(P6);
    add(subSize, P1, 0, P2, 0, result, findIndex(result, size, 0, 2));
    deleteMatrix(P2);
    add(subSize, P3, 0, P4, 0, result, findIndex(result, size, 0, 3));
    deleteMatrix(P4);
    multiOperations(subSize, P5, 0, P1, 0, P3, 0, P7, 0, result, findIndex(result, size, 0, 4), '-');
    deleteMatrix(P1);
    deleteMatrix(P3);
    deleteMatrix(P5);
    deleteMatrix(P7);
    return result;
}