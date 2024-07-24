cmake_minimum_required(VERSION 3.0)

project(GLExperiments)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(TO_NATIVE_PATH "${STAGED_INSTALL_PREFIX}/Libs/")

set(SOURCES makingawindow.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
