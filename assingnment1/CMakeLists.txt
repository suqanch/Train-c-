cmake_minimum_required(VERSION 2.8)

project(assignmen1)

add_executable(RailwayWorld.out RailwayWorld.cpp RailModels.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries( ass1.out ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
