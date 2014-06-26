cmake_minimum_required (VERSION 2.8)
project (fmi_cosim)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-files)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fpermissive -std=c++0x")
set_directory_properties(PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/bin)

include_directories(include)
ADD_SUBDIRECTORY(src)
