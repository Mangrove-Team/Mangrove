cmake_minimum_required(VERSION 4.1)
project(Mangrove)

set(CMAKE_CXX_STANDARD 14)

add_executable(Mangrove
        src/main.cpp
        src/Plan/Plan.cpp
        src/Stand/Stand.cpp
        src/Constraint/AConstraint.cpp
        src/Constraint/PmrAccess/PmrAccess.cpp
        src/Constraint/Electricity/Electricity.cpp)

include_directories(src/Stand
                    src/Constraint)