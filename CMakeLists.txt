cmake_minimum_required(VERSION 3.16)

file(GLOB SOURCES "src/*.cpp")

include_directories(inc)
include_directories(libs/Picture/inc)
include_directories(libs/Signal/inc)

find_library(PICTURE_LIB NAMES picture PATHS "libs/Picture/build" NO_DEFAULT_PATH)
find_library(SIGNAL_LIB NAMES signal PATHS "libs/Signal/build" NO_DEFAULT_PATH)

project(picProcessing)

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(ImageMagick COMPONENTS Magick++)
include_directories(${ImageMagick_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PUBLIC ${PICTURE_LIB} ${SIGNAL_LIB} ${ImageMagick_LIBRARIES})
