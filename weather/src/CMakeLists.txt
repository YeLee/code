set(SRC_LIST curl.c json.c mem.c parse.c session.c static.c weather.c)

include_directories(${JSONC_INCLUDE_DIRS})
include_directories(${CURL_INCLUDE_DIRS})

add_executable(weather ${SRC_LIST})
target_link_libraries(weather ${JSONC_LIBRARIES} ${CURL_LIBRARIES})
install(TARGETS weather RUNTIME DESTINATION bin)
