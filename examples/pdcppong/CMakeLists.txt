project(PlaydateCPPong)
add_playdate_application(pdcppong)
file(GLOB_RECURSE PDCPPONG_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*)
target_sources(pdcppong PUBLIC ${PDCPPONG_SRC})

target_link_libraries(pdcppong PUBLIC pdcpp)
