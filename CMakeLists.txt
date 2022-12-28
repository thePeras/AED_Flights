cmake_minimum_required(VERSION 3.24)
project(AED_Aeroportos)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(Classes)

add_executable(AED_Aeroportos
        Classes/Airline.cpp
        Classes/Airline.h
        Classes/Airport.cpp
        Classes/Airport.h
        Classes/Comparators.cpp
        Classes/Flight.cpp
        Classes/Flight.h
        Classes/Graph.cpp
        Classes/Graph.h
        Classes/Location.cpp
        Classes/Location.h
        Classes/Managing.cpp
        Classes/Managing.h
        main.cpp
        menu.cpp
        menu.h
        MenuOption.h
        MenuTwo.h)
