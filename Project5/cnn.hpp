#pragma once
#pragma GCC optimize(3, "Ofast", "inline")
#include "matrix.hpp"
#ifdef WITH_Open_BLAS
#include <cblas.h>
#endif
#include <opencv4/opencv2/opencv.hpp>

typedef struct conv_param
{
    int pad;
    int stride;
    int kernel_size;
    int in_channels;
    int out_channels;
    float *p_weight;
    float *p_bias;
} conv_param;

typedef struct fc_param
{
    int in_features;
    int out_features;
    float *p_weight;
    float *p_bias;
} fc_param;

void faceDetect(string &path);
Matrix<float> readImag(string &fileName);
Matrix<float> ConvBNRelu(Matrix<float> &matrix, conv_param &param);
Matrix<float> ConvBNRelu_BruteForce(Matrix<float> &matrix, conv_param &param);
Matrix<float> MaxPool(Matrix<float> &input);
Matrix<float> full_connected(Matrix<float> &input, fc_param &param);
Matrix<float> im2col(Matrix<float> &im, int pad, int stride, int kernel_size, int in_channels);
