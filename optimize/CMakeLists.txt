cmake_minimum_required(VERSION 2.8.9)
project (RGBConversion)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb -Wall")

include_directories(.)

add_executable(optimized_rgb2ycc rgb2ycc.c libattopng.c) 
target_link_libraries(optimized_rgb2ycc)
add_executable(optimized_ycc2rgb ycc2rgb.c libattopng.c)
target_link_libraries(optimized_ycc2rgb)  
add_executable(optimized_conversion conversion.c libattopng.c)
target_link_libraries(optimized_conversion)
 
