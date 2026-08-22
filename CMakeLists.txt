cmake_minimum_required(VERSION 3.15)
project(CarRentalSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_executable(CarRentalSystem ${SOURCES})

# Target directories configurations (e.g. data folder in current path)
target_include_directories(CarRentalSystem PRIVATE src)
