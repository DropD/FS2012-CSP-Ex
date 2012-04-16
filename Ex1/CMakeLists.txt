cmake_minimum_required (VERSION 2.8)

project (CSP_Ising)

add_executable(ising main.cpp)

target_link_libraries (ising boost_timer-mt)
target_link_libraries (ising boost_system-mt)

add_definitions(-DBOOST_DISABLE_ASSERTS)
if(mario-random)
    add_definitions(-DMARIO_RANDOM)
endif(mario-random)

set(cmake_build_type release cache force)
set(cmake_cxx_flags_release "-O3 -funroll-loops")
