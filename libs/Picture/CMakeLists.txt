cmake_minimum_required(VERSION 3.16)

file(GLOB SOURCES "src/*.cpp")
include_directories(inc)

project(picture)

add_executable(${PROJECT_NAME} ${SOURCES})


#add_library(${PROJECT_NAME} SHARED ${SOURCES})

#target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

find_package(ImageMagick COMPONENTS Magick++)
include_directories(${ImageMagick_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${ImageMagick_LIBRARIES})
