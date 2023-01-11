cmake_minimum_required(VERSION 3.10)
project(qm)
add_executable(test1 test1.cpp)
target_link_libraries(test1 qm)

enable_testing()

add_test(
    NAME Test1
    COMMAND test1
)

