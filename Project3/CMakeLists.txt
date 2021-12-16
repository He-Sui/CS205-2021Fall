cmake_minimum_required(VERSION 3.16)

project(matmul)

include_directories(/opt/OpenBLAS/include)

link_libraries(/opt/OpenBLAS/lib/libopenblas.so)

aux_source_directory(. DIR_SRCS)

add_executable(matmul ${DIR_SRCS})