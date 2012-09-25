cmake_minimum_required(VERSION 2.8)

project(nfp)

set(CMAKE_CXX_FLAGS "-m32 -pipe -combine -ansi -Wall -Werror -fno-strict-aliasing -std=c++0x")

set(CMAKE_CXX_FLAGS_DEBUG "-fno-omit-frame-pointer -fno-inline-functions -fno-inline-functions-called-once -fno-optimize-sibling-calls -fno-default-inline -fno-inline -g -D_GLIBCXX_PROFILE")

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

set(QT_INCLUDE_DIR "/Library/Frameworks/QtCore.framework/Headers")
set(GLOGS_LIB_DIR "")

add_subdirectory(core)
add_subdirectory(app)
add_subdirectory(algos)
