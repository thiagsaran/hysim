ADD_EXECUTABLE(cosim_main
                            main_cosim.cpp
                            support_cosim.cpp
                            stack.cpp
			    xml_parser.cpp
                            )
                    
target_link_libraries(cosim_main
			expat	
			
                     )
