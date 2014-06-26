add_executable(cosim_main 
main_cosim.cpp cosim.cpp stack.cpp support_cosim.cpp xml_parser.cpp)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/lib)
target_link_libraries(cosim_main expat)
add_library(cosim_static STATIC 
cosim.cpp stack.cpp support_cosim.cpp xml_parser.cpp)
add_library(cosim SHARED 
cosim.cpp stack.cpp support_cosim.cpp xml_parser.cpp)

target_link_libraries(cosim expat)
target_link_libraries(cosim_static expat)
target_link_libraries(cosim_main expat)