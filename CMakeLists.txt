cmake_minimum_required(VERSION 3.16)
project(Sandbox VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Sandbox main.cpp "Application/Application.h" "Application/Application.cpp")
target_include_directories(Sandbox PUBLIC Application)


add_subdirectory(VolumeRegistration)
target_link_libraries(Sandbox PUBLIC VolumeRegistration)

add_subdirectory(WGPUF)
target_link_libraries(Sandbox PUBLIC WGPUframework)
target_include_directories(Sandbox PUBLIC "${PROJECT_SOURCE_DIR}/WGPUF")

target_copy_webgpu_binaries(Sandbox)


set(ITK_DIR "D:\\BP\\itk\\ITK-build")

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})
target_link_libraries(Sandbox PUBLIC ${ITK_LIBRARIES})

configure_file("SliceView.wgsl" "SliceView.wgsl")