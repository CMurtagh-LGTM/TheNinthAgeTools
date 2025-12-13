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

add_executable(listgen
    src/listgen.cpp
    src/wasm.cpp
    src/dom.cpp
)
target_include_directories(listgen PUBLIC include)
target_compile_options(listgen PRIVATE -Wall -Wextra -Wpedantic -flto -fno-exceptions -O3)
set_target_properties(listgen PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    # COMPILE_WARNING_AS_ERROR ON
)

target_link_libraries(listgen PRIVATE tomlplusplus::tomlplusplus)

