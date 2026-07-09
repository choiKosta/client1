cmake_minimum_required(VERSION 3.10)
project(client1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(control STATIC src/control/ControlClient.cpp)
target_include_directories(control PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(control_test tests/control_test.cpp)
target_link_libraries(control_test PRIVATE control)

enable_testing()
add_test(NAME control_test COMMAND control_test)
