cmake_minimum_required(VERSION 2.8)

project(nfp)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules)

find_package(Qt4 REQUIRED)
set(QT_DONT_USE_QTGUI 1)
find_package(Glog REQUIRED)


# Use -DCMAKE_CXX_FLAGS="-m32" to build a 32b binary

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -combine -ansi -Wall -Wextra -Werror -std=c++0x")

set(CMAKE_CXX_FLAGS_DEBUG "-fno-omit-frame-pointer -fno-inline-functions -fno-inline-functions-called-once -fno-optimize-sibling-calls -fno-default-inline -fno-inline -g -D_GLIBCXX_PROFILE")

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

add_subdirectory(core)
add_subdirectory(app)
add_subdirectory(algos)
