cmake_minimum_required(VERSION 3.10)

# Name deines Projekts
project(Dataset)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Standardmäßig auf Release setzen (Optimierung 's' bzw. 'O3'), falls nichts anderes angegeben wird
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
    # xfeatures2d # (Hinweis: xfeatures2d ist oft in opencv_contrib auskommentieren, falls es beim Bauen fehlt)
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

# --- TEST BEREICH ---

# Aktiviert die Test-Funktionalität in CMake (ermöglicht 'make test')
enable_testing()

# Sucht nach der installierten Google Test Bibliothek
find_package(GTest REQUIRED)

# Definiert die ausführbare Datei für die Tests
# Wichtig: Wir kompilieren den Test und die zu testende Klasse (DatasetGenerator.cpp), aber NICHT main.cpp!
add_executable(DatasetTest 
    Test/DatasetGeneratorTest.cpp 
    Dataset/DatasetGenerator.cpp
)

# Übergibt den Quellcode-Pfad auch an die Tests (für set_path)
target_compile_definitions(DatasetTest PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

# Sagt dem Compiler, wo er die Header-Datei (DatasetGenerator.hpp) findet
target_include_directories(DatasetTest PRIVATE 
    Dataset 
    ${OpenCV_INCLUDE_DIRS}
)

# Verknüpft die Tests mit GTest (inkl. einer automatischen main-Funktion von GTest), OpenCV und JSON
target_link_libraries(DatasetTest PRIVATE 
    GTest::gtest_main 
    ${OpenCV_LIBS} 
    nlohmann_json::nlohmann_json
)

# Registriert das Test-Programm offiziell im CMake-Testsystem
add_test(NAME DatasetTest COMMAND DatasetTest)