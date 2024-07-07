cmake_minimum_required(VERSION 3.16.3)

project(sobo22)
SET(CMAKE_BUILD_TYPE "Release")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

add_executable(main
    cec22_test_func.cpp 
    de.h 
    tadesse.cpp
    main.cpp 
    search_algorithm.cpp 
)
