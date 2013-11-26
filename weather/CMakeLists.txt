cmake_minimum_required(VERSION 2.8)
project(weather)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(FindPkgConfig)
find_package(JSONC REQUIRED)
find_package(CURL REQUIRED)
add_subdirectory(src)
