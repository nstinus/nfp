cmake_minimum_required(VERSION 2.8)

project(nfp CXX)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules)

find_package(Glog REQUIRED)

set(NFP_CORE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/core)
set(NFP_ALGOS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/algos)
set(NFP_APP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/app)


# Use -DCMAKE_CXX_FLAGS="-m32" to build a 32b binary

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -Wall -Wextra -std=c++0x")
set(CMAKE_CXX_FLAGS_RELEASE "-Werror -DNDEBUG -O3")

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    if (${ALL_WARNINGS})
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Weverything")
    endif ()
elseif (CMAKE_COMPILER_IS_GNUCXX)
   set(CMAKE_CXX_FLAGS "-combine -ansi ${CMAKE_CXX_FLAGS}")
   set(CMAKE_CXX_FLAGS_DEBUG "-fno-omit-frame-pointer -fno-inline-functions -fno-inline-functions-called-once -fno-optimize-sibling-calls -fno-default-inline -fno-inline -g -D_GLIBCXX_PROFILE")
endif ()


if (${BUILD_TESTS})
   enable_testing()
   set(GTEST_LIB gtest)
   add_subdirectory(${GTEST_DIR} ${GTEST_LIB})
endif(${BUILD_TESTS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(core)
add_subdirectory(algos)
add_subdirectory(app)

# INCLUDE(InstallRequiredSystemLibraries)

# SET(CPACK_PACKAGE_VERSION_MAJOR "0")
# SET(CPACK_PACKAGE_VERSION_MINOR "2")
# SET(CPACK_PACKAGE_VERSION_PATCH "2-20-g222aa09")
#
# set(CPACK_GENERATOR "TBZ2")
# # set(CPACK_STRIP_FILES "bin/nfpshm")
#
# include(CPack)
