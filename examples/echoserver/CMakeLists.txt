project(echoserver)
cmake_minimum_required(VERSION 2.6)
include(CMakeBase)

aux_source_directory(. SOURCES)

include_directories(${PROJECT_SOURCE_DIR}/../../include ${PROJECT_SOURCE_DIR})

link_directories("${CMAKE_BINARY_DIR}/tasks")

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} tasks ev)

if(WITH_PROFILER MATCHES "y" OR WITH_PROFILER MATCHES "Y")
  add_definitions(-DPROFILER)
  target_link_libraries(${PROJECT_NAME} profiler)
endif(WITH_PROFILER MATCHES "y" OR WITH_PROFILER MATCHES "Y")
