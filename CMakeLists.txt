cmake_minimum_required(VERSION 4.0)
project(esc-configdata-gen CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(argparse REQUIRED)
find_package(reflectcpp REQUIRED CONFIG)

# compile only the module interface as the library's source
add_library(esc-configdata src/esc-configdata.cppm src/esc-configdata.cpp)
target_sources(esc-configdata PUBLIC
    FILE_SET esc_configdata_modules TYPE CXX_MODULES FILES src/esc-configdata.cppm
)
# compile only the util module interface as the library's source
add_library(util src/util.cppm src/util.cpp)
target_sources(util PUBLIC
    FILE_SET util_modules TYPE CXX_MODULES FILES src/util.cppm
)
target_link_libraries(esc-configdata PUBLIC reflectcpp::reflectcpp)

add_executable(esc-configdata-gen src/main.cpp)

target_link_libraries(
    esc-configdata-gen PRIVATE argparse::argparse esc-configdata
                               reflectcpp::reflectcpp util)

if (NOT BUILD_TESTING STREQUAL OFF)
    enable_testing()
    add_subdirectory(tests)
endif ()

install(
    TARGETS esc-configdata-gen
    DESTINATION "."
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib)
