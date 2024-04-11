cmake_minimum_required(VERSION 3.16)
project(Sandbox VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Sandbox main.cpp)

add_subdirectory(VolumeRegistration)
target_link_libraries(Sandbox PUBLIC VolumeRegistration)

set(ITK_DIR "D:\\BP\\itk\\ITK-build")

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})
target_link_libraries(Sandbox PUBLIC ${ITK_LIBRARIES})