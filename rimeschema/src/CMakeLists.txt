set(SRC_LIST front.c rimecfg.c rimeschema.c)
if(NOT NOLIBYAML)
	set(CMAKE_EXE_LINKER_FLAGS
		"${CMAKE_EXE_LINKER_FLAGS} -L${PROJECT_BINARY_DIR}/lib/yaml -lyaml")
endif(NOT NOLIBYAML)
add_executable(rimeschema ${SRC_LIST})
if(USE_GTK3)
	target_link_libraries(rimeschema ${GTK3_LIBRARIES})
else(NOT USE_GTK3)
	target_link_libraries(rimeschema ${GTK2_LIBRARIES})
endif(USE_GTK3)
install(TARGETS rimeschema RUNTIME DESTINATION bin)
