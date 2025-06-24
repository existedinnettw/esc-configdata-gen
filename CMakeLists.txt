cmake_minimum_required(VERSION 3.15)
project(esc-configdata-gen CXX)

find_package(argparse REQUIRED)
find_package(magic_enum REQUIRED)
find_package(reflectcpp REQUIRED CONFIG)

add_library(esc-configdata src/esc-configdata.cpp)
target_include_directories(
    esc-configdata PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
                          $<INSTALL_INTERFACE:src>)
# target_link_libraries(esc-configdata PUBLIC reflectcpp::reflectcpp)

add_executable(esc-configdata-gen src/main.cpp)

target_link_libraries(
    esc-configdata-gen PRIVATE argparse::argparse esc-configdata
                               magic_enum::magic_enum)

add_subdirectory(tests)

install(
    TARGETS esc-configdata-gen
    DESTINATION "."
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib)
