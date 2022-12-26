cmake_minimum_required(VERSION 3.24)
project(AED2)

set(CMAKE_CXX_STANDARD 14)

include_directories(Classes)

# Doxygen Build
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(BUILD_DOC_DIR "${CMAKE_SOURCE_DIR}/docs/html")
    if(NOT EXISTS "${BUILD_DOC_DIR}")
        file(MAKE_DIRECTORY "${BUILD_DOC_DIR}")
    endif()

    set(DOXYGEN_IN "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile")
    set(DOXYGEN_OUT "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
    configure_file("${DOXYGEN_IN}" "${DOXYGEN_OUT}" @ONLY)

    message("Doxygen build started")
    add_custom_target(Doxygen ALL
            COMMAND "${DOXYGEN_EXECUTABLE}" "${DOXYGEN_OUT}"
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Classes"
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
else(DOXYGEN_FOUND)
    message("Doxygen needs to be installed to generate the documentation.")
endif(DOXYGEN_FOUND)


add_executable(AED2
        Classes/Airline.cpp
        Classes/Airline.h
        Classes/Airport.cpp
        Classes/Airport.h
        Classes/Flight.cpp
        Classes/Flight.h
        Classes/Location.cpp
        Classes/Location.h
        Classes/Managing.cpp
        Classes/Managing.h
        main.cpp Classes/Graph.h Classes/Graph.cpp)
