cmake_minimum_required(VERSION 3.17)
project(GLExperiments)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)





add_executable(window "${CMAKE_CURRENT_SOURCE_DIR}/makingawindow.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/shader.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/Libs/glad.c")
target_include_directories(window PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/Includes")
target_link_directories(window PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/Libs")
target_link_libraries(window "-lglfw3")