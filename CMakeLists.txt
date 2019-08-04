cmake_minimum_required(VERSION 2.8.9)
project (RGBConversion)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb -Wall")

include_directories(.)

add_executable(working_conversion rgb2ycc.c libattopng.c) 
target_link_libraries(working_conversion)
add_executable(optimized_conversion conversion.c libattopng.c)
target_link_libraries(optimized_conversion)
 
