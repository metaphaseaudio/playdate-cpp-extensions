project(PlaydateCPPExtensionsSynthesis)
add_playdate_application(pdcpp_example_synthesis)
file(GLOB_RECURSE PDCPP_EXAMPLE_SYNTHESIS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*)
target_sources(pdcpp_example_synthesis PUBLIC ${PDCPP_EXAMPLE_SYNTHESIS_SRC})

target_link_libraries(pdcpp_example_synthesis PUBLIC pdcpp)
