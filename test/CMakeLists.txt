cmake_minimum_required(VERSION 3.12)

project(ld2410x-test
  LANGUAGES   C
  DESCRIPTION "ld2410x library testing."
)

add_library(common_lib STATIC common.c)
target_link_libraries(common_lib ld2410x unity)

file(GLOB SOURCES test_*.c)

add_subdirectory(unity)

enable_testing()

foreach(source_file ${SOURCES})
    get_filename_component(executable_name ${source_file} NAME_WE)
    add_executable(${executable_name} ${source_file})
    target_link_libraries(${executable_name} ld2410x unity common_lib)

    add_test(
        NAME ${executable_name}
        COMMAND ${executable_name}
    )
endforeach()
