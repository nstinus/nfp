# FIXME: get rid of this
set(PYTHON_LIBRARY /Library/Frameworks/Python.framework/Versions/2.7/lib/libpython2.7.dylib)
set(PYTHON_INCLUDE_DIR /Library/Frameworks/Python.framework/Headers)
find_package(PythonLibs REQUIRED)

find_package(Swig REQUIRED)
include(${SWIG_USE_FILE})

include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

set(CMAKE_SWIG_FLAGS "")

set_source_files_properties(nfp.i PROPERTIES CPLUSPLUS ON)
swig_add_module(nfp python nfp.i)
set_source_files_properties(${swig_generated_file_fullname} COMPILE_FLAGS "-Wno-self-assign")
swig_link_libraries(nfp
  ${PYTHON_LIBRARIES}
  ${NFP_CORE_LIBRARY}
  ${NFP_ALGOS_LIBRARY}
  ${GLOG_LIBRARY}
)