cmake_minimum_required(VERSION 3.20)
project(sylar)

include(cmake/utils.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++11 -Wall -Wno-deprecated -Wno-unused-function -Wno-builtin-macro-redefined")

include_directories(.)
include_directories(/usr/local/include)

find_library(YAMLCPP yaml-cpp)

link_directories(/usr/local/lib)

set(LIB_SRC
        sylar/log.cc
        sylar/util.cc
        sylar/config.cc
        sylar/thread.cc
        sylar/fiber.cc
        sylar/scheduler.cc
        sylar/mutex.cc
    )

add_library(sylar SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(sylar)  # 重定义__FILE__宏，保护绝对路径

set(LIB_LIB
        sylar
        pthread
        ${YAMLCPP}
)

add_executable(test tests/test.cc) 
add_dependencies(test sylar)
force_redefine_file_macro_for_sources(test)  # 重定义__FILE__宏，保护绝对路径
target_link_libraries(test sylar ${LIB_LIB})

add_executable(test_config tests/test_config.cc)
add_dependencies(test_config sylar)
force_redefine_file_macro_for_sources(test_config)  # 重定义__FILE__宏，保护绝对路径
# target_link_libraries(test_config sylar -L/usr/local/lib -lyaml-cpp)
target_link_libraries(test_config sylar ${LIB_LIB})

add_executable(test_thread tests/test_thread.cc)
add_dependencies(test_thread sylar)
force_redefine_file_macro_for_sources(test_thread)
target_link_libraries(test_thread sylar ${LIB_LIB})

add_executable(test_util tests/test_util.cc)
add_dependencies(test_util sylar)
force_redefine_file_macro_for_sources(test_util)
target_link_libraries(test_util sylar ${LIB_LIB})

add_executable(test_fiber tests/test_fiber.cc)
add_dependencies(test_fiber sylar)
force_redefine_file_macro_for_sources(test_fiber)
target_link_libraries(test_fiber sylar ${LIB_LIB})

add_executable(test_scheduler tests/test_scheduler.cc)
add_dependencies(test_scheduler sylar)
force_redefine_file_macro_for_sources(test_scheduler)
target_link_libraries(test_scheduler sylar ${LIB_LIB})



SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)