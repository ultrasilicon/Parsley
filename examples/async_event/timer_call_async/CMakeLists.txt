cmake_minimum_required(VERSION 3.9)
get_filename_component(PROJECT_DIR_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
project(${PROJECT_DIR_NAME})

set(SRC ${PROJECT_SOURCE_DIR}/main.cpp)
set(CMAKE_CXX_STANDARD 14)

if(APPLE) # macOS mojave 10.14+ needs link dir and include dir
    link_directories(/usr/local/lib)
    include_directories(/usr/local/include)
endif()

find_package(PkgConfig)
pkg_check_modules(LIBPARSLEY REQUIRED libagio)

add_executable(${PROJECT_DIR_NAME} main.cpp)
target_link_libraries(${PROJECT_DIR_NAME} agio)
