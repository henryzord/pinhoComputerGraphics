cmake_minimum_required(VERSION 3.10)
project(computer_graphics VERSION 1.0 LANGUAGES CXX)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(JPEG REQUIRED)

add_subdirectory(SOIL)
add_subdirectory(src)
add_subdirectory(examples)
