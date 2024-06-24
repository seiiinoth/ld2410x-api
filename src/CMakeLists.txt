cmake_minimum_required(VERSION 3.12)

project(ld2410x
  LANGUAGES   C
  DESCRIPTION "ld2410x microwave sensor library."
)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
  PRIVATE
  ld2410x.c
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

set(${PROJECT_NAME}_PUBLIC_HEADERS
  ld2410x.h
  ld2410x_defs.h
)

set_target_properties(${PROJECT_NAME}
  PROPERTIES
    C_STANDARD          11
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS        OFF
)

target_compile_options(${PROJECT_NAME}
  PRIVATE
    # Clang
    $<$<C_COMPILER_ID:Clang>:
      -Wcast-align
      -Wcast-qual
      -Wconversion
      -Wexit-time-destructors
      -Wglobal-constructors
      -Wmissing-noreturn
      -Wmissing-prototypes
      -Wno-missing-braces
      -Wold-style-cast
      -Wshadow
      -Wweak-vtables
      -Werror
      -Wall
      $<$<VERSION_GREATER_EQUAL:$<C_COMPILER_VERSION>,8.0.0>:-Wextra-semi-stmt>
    >

    # GCC
    $<$<C_COMPILER_ID:GNU>:
      -Waddress
      -Waggregate-return
      -Wformat-nonliteral
      -Wformat-security
      -Wformat
      -Winit-self
      -Wmissing-declarations
      -Wmissing-include-dirs
      -Wno-multichar
      -Wno-parentheses
      -Wno-type-limits
      -Wno-unused-parameter
      -Wunreachable-code
      -Wwrite-strings
      -Wpointer-arith
      -Werror
      -Wall
    >

    # MSVC
    $<$<C_COMPILER_ID:MSVC>:
      /Wall
    >

    $<$<CONFIG:DEBUG>:-g3>
    $<$<CONFIG:DEBUG>:-Og>
    $<$<CONFIG:RELEASE>:-O3>
)

## Target installation
install(TARGETS   ${PROJECT_NAME}
    EXPORT        ${PROJECT_NAME}Targets
    ARCHIVE       DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    COMPONENT     library
)
