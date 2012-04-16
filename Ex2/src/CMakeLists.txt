set(cmake_include_current_dir on)

add_executable(ex2 ex2.cpp)

target_link_libraries (ex2 boost_timer-mt)
target_link_libraries (ex2 boost_system-mt)

add_definitions(-DBOOST_DISABLE_ASSERTS)

set(cmake_build_type release cache force)
set(cmake_cxx_flags "-O3 -funroll-loops")
set(cmake_cxx_flags_release "-O3 -funroll-loops")
