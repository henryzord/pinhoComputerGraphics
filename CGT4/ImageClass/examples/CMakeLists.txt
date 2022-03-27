set(LIST_INCLUDE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/SOIL/include
    ${OPENGL_INCLUDE_DIR}
    ${GLUT_INCLUDE_DIR}
    ${JPEG_INCLUDE_DIR})

set(LIST_LINK
    utils
    soil
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
    ${JPEG_LIBRARIES})

file(GLOB LIST_EXAMPLES ${CMAKE_SOURCE_DIR}/examples/*.cpp)
foreach(EXAMPLE ${LIST_EXAMPLES})
    get_filename_component(EXAMPLE_FILENAME ${EXAMPLE} NAME)
    string(REPLACE ".cpp" "" EXAMPLE_NAME ${EXAMPLE_FILENAME})

    add_executable(${EXAMPLE_NAME} ${EXAMPLE_FILENAME})
    target_include_directories(${EXAMPLE_NAME} PRIVATE ${LIST_INCLUDE})
    target_link_libraries(${EXAMPLE_NAME} PRIVATE ${LIST_LINK})
endforeach(EXAMPLE)
