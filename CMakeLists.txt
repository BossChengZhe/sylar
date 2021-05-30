cmake_minimum_required(VERSION 3.20)
project(sylar)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")

include_directories(.)
include_directories(/usr/local/include)

find_library(YAMLCPP yaml-cpp)

link_directories(/usr/local/lib)

set(LIB_SRC
        sylar/log.cc
        sylar/util.cc
        sylar/config.cc
    )

add_library(sylar SHARED ${LIB_SRC})

add_executable(test tests/test.cc)
add_dependencies(test sylar)
target_link_libraries(test sylar ${YAMLCPP})

add_executable(test_config tests/test_config.cc)
add_dependencies(test_config sylar)
target_link_libraries(test_config sylar -L/usr/local/lib -lyaml-cpp)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)