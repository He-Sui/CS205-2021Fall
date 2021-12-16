cmake_minimum_required(VERSION 3.10)

project(matrix)

aux_source_directory(. DIR_SRCS)

add_executable(matrix ${DIR_SRCS})

