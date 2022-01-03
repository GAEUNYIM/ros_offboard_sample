cmake_minimum_required(VERSION 3.0.2)
project(ros_offboard_sample)

add_compile_options(-std=c++17)

find_package(catkin_simple REQUIRED)
catkin_simple(ALL_DEPS_REQUIRED)

cs_add_executable(sample
    src/sample.cpp
)

cs_install()
cs_export()
