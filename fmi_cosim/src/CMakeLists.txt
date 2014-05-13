ADD_EXECUTABLE(fmi_cosim
                            main_cosim.c
                            support_cosim.c
                            stack.c
			    xml_parser.c
                            )
                    
target_link_libraries(fmi_cosim
			expat	
			
                     )
