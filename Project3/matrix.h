#pragma once

#pragma GCC optimize(3, "Ofast", "inline")
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

struct Matrix
{
    int row;
    int column;
    float *data;
};

struct Matrix *creatMatrix(const int, const int, float *const);
void deleteMatrix(struct Matrix *);
void copyMatrix(const struct Matrix *const, struct Matrix *const);
struct Matrix *multiply(const struct Matrix *const, const struct Matrix *const);
void printMatrix(const struct Matrix *const, FILE *);
void transpose(struct Matrix *const);
struct Matrix *readFile(FILE *);
void *add(int size, struct Matrix *, int, struct Matrix *, int, struct Matrix *, int);
void *minus(int, struct Matrix *, int, struct Matrix *, int, struct Matrix *, int);
int findIndex(struct Matrix *, int, int, int);
void *multiOperations(int, struct Matrix *, int, struct Matrix *, int, struct Matrix *, int, struct Matrix *, int, struct Matrix *, int, char);
struct Matrix *strassen(struct Matrix *, int, struct Matrix *, int, int);