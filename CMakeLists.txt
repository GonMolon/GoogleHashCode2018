cmake_minimum_required(VERSION 3.8)
project(GoogleHashCode)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE ReleaseWithAsserts)
set(CMAKE_CXX_FLAGS_RELEASEWITHASSERTS "-Wall -O3")

set(SOURCE_FILES
        local_search/HillClimbing.cpp
        local_search/HillClimbing.h
        local_search/LocalSearch.cpp
        local_search/LocalSearch.h
        local_search/SimulatedAnnealing.cpp
        local_search/SimulatedAnnealing.h
        local_search/State.h
        TestState.cpp
        TestState.h
        main.cpp)

add_executable(GoogleHashCode ${SOURCE_FILES})