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
# FetchContent_Declare(
#     webgpu-headers
#     GIT_REPOSITORY https://github.com/webgpu-native/webgpu-headers.git
#     GIT_TAG main
#     GIT_SHALLOW TRUE
#     SOURCE_SUBDIR NONE
# )
# FetchContent_MakeAvailable(webgpu-headers tomlplusplus)

add_executable(listgen src/listgen.cpp)
target_include_directories(listgen PUBLIC include)
target_compile_options(listgen PRIVATE -Wall -Wextra -Wpedantic -flto)
set_target_properties(listgen PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    COMPILE_WARNING_AS_ERROR ON
)

target_link_libraries(listgen PRIVATE tomlplusplus::tomlplusplus)
# target_include_directories(listgen SYSTEM PRIVATE
#   "${FETCHCONTENT_BASE_DIR}/webgpu-headers-src/"
# )
