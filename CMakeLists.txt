cmake_minimum_required(VERSION 3.10)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "vcpkg toolchain")
endif()

project(client1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(control STATIC src/control/ControlClient.cpp)
target_include_directories(control PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(streaming STATIC src/streaming/StreamClient.cpp)
target_include_directories(streaming PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(control_test tests/control_test.cpp)
target_link_libraries(control_test PRIVATE control)

add_executable(streaming_test tests/streaming_test.cpp)
target_link_libraries(streaming_test PRIVATE streaming)

add_executable(control_network_smoke tests/control_network_smoke.cpp)
target_link_libraries(control_network_smoke PRIVATE control)

enable_testing()
add_test(NAME control_test COMMAND control_test)
add_test(NAME streaming_test COMMAND streaming_test)
