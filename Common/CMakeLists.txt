cmake_minimum_required (VERSION 2.8)

set(cmake_release_type Release cache force)
project (ND_Lattice)

add_executable(test test_common.cpp)

target_link_libraries(test boost_timer-mt)
target_link_libraries(test boost_system-mt)
target_link_libraries(test gomp)

#link_libraries(boost_python-mt)
#include_directories("/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7/")
#link_directories("/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/")
#add_library(pyIsing SHARED pyIsing.cpp)

add_definitions(-DBOOST_DISABLE_ASSERTS)

set(cmake_cxx_flags "-fopenmp")
set(cmake_c_flags "-fopenmp")

set(cmake_cxx_flags_release "-O3 -funroll-loops -fopenmp")
