project("Playdate CPP Extensions")


file(GLOB_RECURSE PD_CPP_EXT_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(pdcpp STATIC ${PD_CPP_EXT_SOURCE})
target_include_directories(pdcpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_link_libraries(pdcpp PUBLIC pdcpp_core)

if (PDCPP_BUILD_EXAMPLES)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)
endif ()

if (PDCPP_BUILD_UTILITIES)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/utilities)
endif ()