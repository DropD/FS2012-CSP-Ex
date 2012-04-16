cmake_minimum_required (VERSION 2.8)

project (CSP_Ising)

add_executable(ising main.cpp)

target_link_libraries (ising boost_timer-mt)
target_link_libraries (ising boost_system-mt)

add_definitions(-DBOOST_DISABLE_ASSERTS)
set(cmake_cxx_flags_distribution "-O3 -funroll-loops")
set(cmake_build_type release)
