set(SRC_LIST weather.c)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lcurl -ljson")
add_executable(weather ${SRC_LIST})
install(TARGETS weather RUNTIME DESTINATION bin)
