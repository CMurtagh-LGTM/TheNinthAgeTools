cmake_minimum_required(VERSION 4.2.0)

project(listgen)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXECUTABLE_SUFFIX ".wasm")

include(FetchContent)
FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG v3.4.0
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(tomlplusplus)

FILE(GLOB webidl_sources CONFIGURE_DEPENDS webidl/*.webidl)
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/browser.wit
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/webidl2wit/target/debug/webidl2wit
        --wit-package browser
        --webidl ${CMAKE_CURRENT_SOURCE_DIR}/webidl/*.webidl
        --phantom-interface Function EventListener
        > ${CMAKE_CURRENT_BINARY_DIR}/browser.wit
    DEPENDS ${webidl_sources}
)

add_custom_command(
    OUTPUT the_ninth_age_tools.cpp the_ninth_age_tools_cpp.h wit.h the_ninth_age_tools_component_type.o
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/wit-bindgen/target/debug/wit-bindgen cpp -w cmurtagh:the-ninth-age-tools/the-ninth-age-tools
        ${CMAKE_CURRENT_SOURCE_DIR}/wit/wasi-wit-0.2.10/io/io.wit
        ${CMAKE_CURRENT_BINARY_DIR}/browser.wit
        ${CMAKE_CURRENT_SOURCE_DIR}/wit/world.wit
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/wit/wasi-wit-0.2.10/io/io.wit
        ${CMAKE_CURRENT_BINARY_DIR}/browser.wit
        ${CMAKE_CURRENT_SOURCE_DIR}/wit/world.wit
)

add_executable(listgen
    src/listgen.cpp
    src/wasm.cpp
    src/dom.cpp
    src/table.cpp
    src/reactive.cpp
    src/unit.cpp
    src/input.cpp
    src/memory_log.cpp
    the_ninth_age_tools.cpp
    the_ninth_age_tools_component_type.o
)
target_include_directories(listgen PUBLIC include)
target_include_directories(listgen PUBLIC build_wasm) # TODO
target_compile_options(listgen PRIVATE -Wall -Wextra -Wpedantic -flto -fno-exceptions -Oz -gline-tables-only)
# target_compile_options(listgen PRIVATE -mexec-model=reactor -Wall -Wextra -Wpedantic -flto -fno-exceptions -Oz -gline-tables-only)
set_target_properties(listgen PROPERTIES
    CXX_STANDARD 26
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    # COMPILE_WARNING_AS_ERROR ON
)

target_link_libraries(listgen PRIVATE tomlplusplus::tomlplusplus)

add_custom_command(
    TARGET listgen
    POST_BUILD
    COMMAND wasm-tools
    ARGS print -o $<TARGET_FILE:listgen>.wat $<TARGET_FILE:listgen>
    COMMENT "Generating wat file"
)

# add_custom_command(
#     TARGET listgen 
#     POST_BUILD
#     COMMAND python
#     ARGS ${PROJECT_SOURCE_DIR}/wasm-sourcemap.py --basepath=${PROJECT_SOURCE_DIR} $<TARGET_FILE:listgen> -o $<TARGET_FILE:listgen>.map
#     COMMENT "Generating map file"
#     # BYPRODUCTS $<TARGET_FILE:listgen>.map
# )

# add_custom_command(
#     TARGET listgen 
#     POST_BUILD
#     COMMAND ${CMAKE_STRIP}
#     ARGS --strip-all $<TARGET_FILE:listgen>
#     COMMENT "Stripping symbols"
# )

