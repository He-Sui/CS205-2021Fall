#pragma once
#include "matrix.hpp"
#include <cblas.h>
#include <opencv4/opencv2/opencv.hpp>
#include "face_binary_cls.hpp"

void faceDetect(string &path);
Matrix<float> readImag(string& fileName);
Matrix<float> ConvBNRelu(Matrix<float>& matrix, conv_param& param);
Matrix<float> ConvBNRelu2(Matrix<float>& matrix, conv_param& param);
Matrix<float> MaxPool(Matrix<float>& input);
Matrix<float> full_connected(Matrix<float> &input, fc_param &param);