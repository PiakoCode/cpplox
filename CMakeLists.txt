cmake_minimum_required(VERSION 3.10.0)
project(cpplox VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

include(CTest)
enable_testing()

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/object/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h" "include/object/*.h")

add_executable(cpplox ${SOURCES} ${HEADERS})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
