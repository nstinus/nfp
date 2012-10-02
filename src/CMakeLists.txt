cmake_minimum_required(VERSION 2.8)

project(nfp)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules)

find_package(Glog REQUIRED)

set(NFP_CORE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/core)
set(NFP_ALGOS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/algos)
set(NFP_APP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/app)


# Use -DCMAKE_CXX_FLAGS="-m32" to build a 32b binary

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -combine -ansi -Wall -Wextra -Werror -std=c++0x")

set(CMAKE_CXX_FLAGS_DEBUG "-fno-omit-frame-pointer -fno-inline-functions -fno-inline-functions-called-once -fno-optimize-sibling-calls -fno-default-inline -fno-inline -g -D_GLIBCXX_PROFILE")

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

if (${BUILD_TESTS})
   enable_testing()
   set(GTEST_LIB gtest)
   add_subdirectory(${GTEST_DIR} ${GTEST_LIB})
endif(${BUILD_TESTS})

add_subdirectory(${NFP_CORE_DIR})
add_subdirectory(${NFP_ALGOS_DIR})
add_subdirectory(${NFP_APP_DIR})
