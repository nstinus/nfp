cmake_minimum_required(VERSION 2.8)

project(nfp CXX)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules)

find_package(Glog REQUIRED)

find_package(Git REQUIRED)
set(CMAKE_EXE_LINKER_FLAGS "-D_NFP_GIT_SHA1=\"${NFP_GIT_SHA1}\" ${CMAKE_EXE_LINKER_FLAGS}")

set(NFP_CORE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/core)
set(NFP_ALGOS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/algos)
set(NFP_APP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/app)


# Use -DCMAKE_CXX_FLAGS="-m32" to build a 32b binary

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -Wall -Wextra -std=c++98")
set(CMAKE_CXX_FLAGS_RELEASE "-Werror -DNDEBUG -O3")

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    if (${ALL_WARNINGS})
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Weverything")
    endif ()
elseif (CMAKE_COMPILER_IS_GNUCXX)
   set(CMAKE_CXX_FLAGS "-combine -ansi ${CMAKE_CXX_FLAGS}")
   set(CMAKE_CXX_FLAGS_DEBUG "-fno-omit-frame-pointer -fno-inline-functions -fno-inline-functions-called-once -fno-optimize-sibling-calls -fno-default-inline -fno-inline -g -D_GLIBCXX_PROFILE")
endif ()


if(IS_DIRECTORY ${GTEST_DIR})
  message(STATUS "Building test suite.")
  include(CTest)
  if (${BUILD_TESTING})
    set(GTEST_LIB gtest)
    add_subdirectory(${GTEST_DIR} ${GTEST_LIB})
  endif(${BUILD_TESTING})
else(IS_DIRECTORY ${GTEST_DIR})
  message(STATUS "Not building test suite (GTEST_DIR not specified or not a directory).")
endif(IS_DIRECTORY ${GTEST_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(NFP_CORE_LIBRARY core)
set(NFP_ALGOS_LIBRARY algos)

add_subdirectory(core)
add_subdirectory(algos)
add_subdirectory(app)
add_subdirectory(python)

INCLUDE(InstallRequiredSystemLibraries)

SET(CPACK_PACKAGE_VERSION_MAJOR ${NFP_MAJOR_VERSION})
SET(CPACK_PACKAGE_VERSION_MINOR ${NFP_MINOR_VERSION})
SET(CPACK_PACKAGE_VERSION_PATCH ${NFP_PATCH_VERSION})

set(CPACK_GENERATOR "TBZ2")
# set(CPACK_STRIP_FILES "bin/nfpshm")

include(CPack)
