 cmake_minimum_required(VERSION 3.16)


file(GLOB SOURCES "src/*.cpp")
include_directories(inc)

project(signal)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
add_executable(signalBin ${SOURCES} main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
