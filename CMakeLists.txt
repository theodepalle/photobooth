cmake_minimum_required(VERSION 3.2)

project(photobooth)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra)

find_library(GPHOTO2_LIB gphoto2 REQUIRED)
find_library(GPHOTO2_PORT_LIB gphoto2_port REQUIRED)

# BUILD
include_directories(include)

file(GLOB_RECURSE SRC_FILES "src/*")
add_executable(photobooth ${SRC_FILES})
target_link_libraries(photobooth ${GPHOTO2_LIB} ${GPHOTO2_PORT_LIB})

# INSTALL
install(TARGETS photobooth DESTINATION bin)

# TESTING
