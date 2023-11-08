project(PlaydateCPPExtensionsHelloWorld)
add_playdate_application(pdcppext_helloworld)
file(GLOB_RECURSE PDCPPEXT_HELLO_WORLD_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*)
target_sources(pdcppext_helloworld PUBLIC ${PDCPPEXT_HELLO_WORLD_SRC})

target_link_libraries(pdcppext_helloworld pdcpp)
