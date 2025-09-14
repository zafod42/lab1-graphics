# CMakeLists.txt
cmake_minimum_required(VERSION 4.0)

project(OpenGLTemplate)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")

file(GLOB PROJECT_SOURCES ${PROJECT_SOURCE_DIR}/*.cc)
file(GLOB PROJECT_HEADERS ${PROJECT_SOURCE_DIR}/*.hh)
file(GLOB GLAD_SOURCES /usr/src/gl.c)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCES} ${GLAD_SOURCES})

target_link_libraries(${PROJECT_NAME} glut GLU GL X11 Xi pthread)

