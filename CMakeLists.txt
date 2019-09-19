cmake_minimum_required(VERSION 2.8)
set(PROJECT_NAME Museum)
project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 11)

include_directories(/usr/include/eigen3)
FIND_PACKAGE(OpenCV REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS})

set(SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/src/calc_path.cpp
        ${PROJECT_SOURCE_DIR}/src/draw_illustration.cpp
        ${PROJECT_SOURCE_DIR}/src/laser_point_prob.cpp
        ${PROJECT_SOURCE_DIR}/src/length.cpp
        ${PROJECT_SOURCE_DIR}/src/main.cpp
        ${PROJECT_SOURCE_DIR}/src/occupied_grid.cpp
        ${PROJECT_SOURCE_DIR}/src/public.cpp
        ${PROJECT_SOURCE_DIR}/src/resampling.cpp
        ${PROJECT_SOURCE_DIR}/src/update_map.cpp
        ${PROJECT_SOURCE_DIR}/src/update_particles.cpp
        )

add_executable(Museum ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(Museum ${OpenCV_LIBS})