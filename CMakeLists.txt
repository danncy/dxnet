PROJECT(dxnet)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
ADD_DEFINITIONS(-std=c++17)
SET(CMAKE_BUILE_TYPE DEBUG)
SET(CMAKE_C_FLAGS_DEBUG "-g -Wall")

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/
    ${PROJECT_SOURCE_DIR}/build/
    ${PROJECT_SOURCE_DIR}/build/third_party/libevent/include
    )

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

ADD_SUBDIRECTORY(framework)
ADD_SUBDIRECTORY(interface)
ADD_SUBDIRECTORY(network_adapter)

LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/lib)
ADD_EXECUTABLE(main main.cpp)
TARGET_LINK_LIBRARIES(main framework pthread event interface)

ADD_EXECUTABLE(adapterd adapterd.cpp)
TARGET_LINK_LIBRARIES(adapterd framework pthread event)

ADD_EXECUTABLE(unittest tests/main.cpp)
TARGET_LINK_LIBRARIES(unittest gtest pthread interface framework event)
