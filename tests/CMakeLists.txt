find_package(GTest REQUIRED)
include(GoogleTest)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(unittests test_gen.cpp)
target_link_libraries(unittests PRIVATE GTest::gtest_main esc-configdata
                                        magic_enum::magic_enum)
target_compile_options(unittests PRIVATE -Wall)

gtest_discover_tests(unittests)
