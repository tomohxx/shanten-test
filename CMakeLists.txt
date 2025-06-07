cmake_minimum_required(VERSION 3.28)

project(shanten-test)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -Wextra -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -Wextra")

find_package(Threads REQUIRED COMPONENTS Threds)
find_package(Calsht REQUIRED PATHS $ENV{HOME}/local/share/cmake)

add_library(shanten STATIC src/shanten.cpp)

add_executable(test1 src/test1.cpp)
target_include_directories(test1 PUBLIC $ENV{HOME}/local/include)
target_link_libraries(test1 shanten calsht)
target_compile_definitions(test1 PUBLIC INDEX_FILE_PATH="$ENV{HOME}/local/share/data")

add_executable(test2 src/test2.cpp)
target_include_directories(test2 PUBLIC $ENV{HOME}/local/include)
target_link_libraries(test2 shanten calsht Threads::Threads)
target_compile_definitions(test2
  PUBLIC INDEX_FILE_PATH="$ENV{HOME}/local/share/data"
  PUBLIC NUM_THREADS=15)

add_executable(test3 src/test3.cpp)
target_include_directories(test3 PUBLIC $ENV{HOME}/local/include)
target_link_libraries(test3 shanten calsht)
target_compile_definitions(test3 PUBLIC INDEX_FILE_PATH="$ENV{HOME}/local/share/data")
