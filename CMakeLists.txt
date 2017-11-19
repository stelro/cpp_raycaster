cmake_minimum_required(VERSION 3.8)
project(raycasting)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)


set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp cglib.cpp cglib.hpp)
add_executable(raycasting ${SOURCE_FILES})

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})

target_link_libraries(raycasting ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})

add_custom_command(TARGET raycasting POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:raycasting>/assets)
