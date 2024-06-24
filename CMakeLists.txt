cmake_minimum_required(VERSION 3.12)

project(ld2410x
  LANGUAGES   C
  DESCRIPTION "ld2410x microwave sensor library."
)

include(CTest)
add_subdirectory(src)
add_subdirectory(test)
