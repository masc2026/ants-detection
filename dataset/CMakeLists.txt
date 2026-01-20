cmake_minimum_required(VERSION 3.10)

project(Dataset)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

find_package(OpenCV REQUIRED COMPONENTS 
    core 
    imgproc 
    highgui 
    video 
    videoio 
    features2d 
    flann 
    imgcodecs
)

find_package(nlohmann_json REQUIRED)

add_executable(Dataset 
    Dataset/main.cpp 
    Dataset/DatasetGenerator.cpp
)

target_compile_definitions(Dataset PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

target_include_directories(Dataset PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(Dataset PRIVATE 
    ${OpenCV_LIBS} 
    nlohmann_json::nlohmann_json
)

# TEST

enable_testing()

find_package(GTest REQUIRED)

add_executable(DatasetTest 
    Test/DatasetGeneratorTest.cpp 
    Dataset/DatasetGenerator.cpp
)

target_compile_definitions(DatasetTest PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

target_include_directories(DatasetTest PRIVATE 
    Dataset 
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(DatasetTest PRIVATE 
    GTest::gtest_main 
    ${OpenCV_LIBS} 
    nlohmann_json::nlohmann_json
)

add_test(NAME DatasetTest COMMAND DatasetTest)