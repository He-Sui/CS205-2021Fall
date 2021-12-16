#include "matrix.h"
#include <stdbool.h>
#include <sys/time.h>
#include <cblas.h>

int main(int args, char **argv)
{
    if (args < 4)
    {
        printf("File missing!\n");
        return 1;
    }
    FILE *input1, *input2, *output;
    input1 = fopen(argv[1], "r");
    input2 = fopen(argv[2], "r");
    if (input1 == NULL || input2 == NULL)
    {
        printf("File does not exist!\n");
        return 1;
    }

    struct Matrix *A = readFile(input1);

    fclose(input1);
    if (A == NULL)
    {
        printf("File1 Corrupted!\n");
        return 1;
    }
    struct Matrix *B = readFile(input2);
    fclose(input2);
    if (B == NULL)
    {
        printf("File2 Corrupted!\n");
        return 1;
    }
    printf("Read File Successfully!\n");

    struct timeval start_time, end_time;
    long long duration;

    gettimeofday(&start_time, NULL);
    struct Matrix *result1 = multiply(A, B);
    gettimeofday(&end_time, NULL);
    if (result1 == NULL)
        return 1;
    duration = 1000000 * (end_time.tv_sec - start_time.tv_sec) + end_time.tv_usec - start_time.tv_usec;
    printf("The calculate time is: %lldms\n", duration / 1000);

    struct Matrix *result2;
    if (A->row == A->column && A->row == B->column && (A->row & (A->row - 1)) == 0 && A->row >= 128)
    {
        gettimeofday(&start_time, NULL);
        result2 = strassen(A, 0, B, 0, A->column);
        gettimeofday(&end_time, NULL);
        duration = 1000000 * (end_time.tv_sec - start_time.tv_sec) + end_time.tv_usec - start_time.tv_usec;
        printf("The strassen calculate time is: %lldms\n", duration / 1000);
    }

    struct Matrix *result3 = creatMatrix(A->row, B->column, (float *)malloc(sizeof(float) * A->row * B->column));
    const float alpha = 1;
    const float beta = 0;
    gettimeofday(&start_time, NULL);
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, A->row, B->column, A->column, alpha, A->data, A->column, B->data, B->column, beta, result3->data, result3->column);
    gettimeofday(&end_time, NULL);
    duration = 1000000 * (end_time.tv_sec - start_time.tv_sec) + end_time.tv_usec - start_time.tv_usec;
    printf("The OpenBLAS calculate time is: %lldms\n", duration / 1000);

    output = fopen(argv[3], "w");
    if (output == NULL)
        printf("File does not exist!\n");
    printMatrix(result1, output);
    fclose(output);
    printf("Write File Successfully!\n");
    return 0;
}