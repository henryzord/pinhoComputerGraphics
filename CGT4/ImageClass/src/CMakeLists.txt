file(GLOB LIST_HEADERS ${CMAKE_SOURCE_DIR}/include/*.h)
file(GLOB LIST_SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_library(utils ${LIST_HEADERS} ${LIST_SOURCES})

target_include_directories(
    utils
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/SOIL/include
    ${OPENGL_INCLUDE_DIR}
    ${GLUT_INCLUDE_DIR}
    ${JPEG_INCLUDE_DIR})

target_link_libraries(
    utils
    PUBLIC
    soil
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
    ${JPEG_LIBRARIES})
