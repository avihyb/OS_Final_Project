cmake_minimum_required(VERSION 3.10)
project(OS_Final_Project)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
add_library(Graph src/Graph.cpp)
add_library(Tree src/Tree.cpp)
add_library(MSTAlgorithm src/MSTAlgorithm.cpp)
add_library(MSTAlgorithmFactory src/MSTAlgorithmFactory.cpp)
add_library(KruskalMST src/KruskalMST.cpp)
add_library(GraphVisualizer src/GraphVisualizer.cpp)

# Test executable
add_executable(tests tests/main.cpp tests/GraphTests.cpp tests/TreeTests.cpp tests/MSTAlgorithmTests.cpp)
target_link_libraries(tests Graph Tree MSTAlgorithm MSTAlgorithmFactory KruskalMST gtest gtest_main)
