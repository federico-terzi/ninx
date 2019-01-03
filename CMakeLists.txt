cmake_minimum_required(VERSION 3.13)
project(ninx)

set(CMAKE_GENERATOR_PLATFORM x64)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost)
IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif ()

set(Boost_USE_STATIC_LIBS ON) # enable static linking
set(Boost_USE_MULTITHREAD ON)  # enable multithreading
set(Boost_ARCHITECTURE "-x64")

find_package(Boost COMPONENTS REQUIRED regex unit_test_framework)

add_subdirectory(lib)
add_subdirectory(cli)
add_subdirectory(test)