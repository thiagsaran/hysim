ADD_EXECUTABLE(fmi_cosim
                            main_cs.c
                            sim_support.c
                            stack.c
			    xml_parser.c
                            )
                    
target_link_libraries(fmi_cosim
			expat	
			
                     )
