
cmake_minimum_required(VERSION 3.13.4)

project(ecs_case_study)
set(CMAKE_CXX_STANDARD 11)

file(GLOB SOURCES "src/**.cpp")
file(GLOB CORE "core_engine/src/**.cpp")
file(GLOB CORE_C "core_engine/src/**.c")
add_executable(${PROJECT_NAME} ${SOURCES} ${CORE} ${CORE_C})
include_directories(headers)
include_directories(core_engine/headers)

target_include_directories(${PROJECT_NAME} PUBLIC "/usr/local/include/")
target_link_libraries(${PROJECT_NAME} "/usr/local/lib/libglfw.3.3.dylib")
target_link_libraries(${PROJECT_NAME} "/usr/local/lib/libassimp.5.3.0.dylib")
