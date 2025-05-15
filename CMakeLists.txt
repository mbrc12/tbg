cmake_minimum_required(VERSION 3.30)
project(TurnBasedGame)

# For tmxlite
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
set(TMXLITE_STATIC_LIB TRUE)
add_subdirectory(deps/tmxlite/tmxlite)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_LINKER_TYPE MOLD)

add_subdirectory(deps/SFML)

set(IMGUI_DIR ${PROJECT_SOURCE_DIR}/deps/imgui)
set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_SFML_IMGUI_DEMO ON)
add_subdirectory(deps/imgui-sfml)

add_subdirectory(deps/backward-cpp)

set(SOURCE_FILES
    src/main.cpp
    src/common.cpp
    src/arch.cpp
    src/input.cpp
    src/events.cpp
    src/assets.cpp

    src/components/timer.cpp
)

add_executable(main ${SOURCE_FILES})
target_include_directories(main PRIVATE src/)
target_compile_features(main PRIVATE cxx_std_20)
# target_compile_options(main PRIVATE "-O0")

set(LIBS
    sfml-graphics
    sfml-window
    sfml-system
    ImGui-SFML
    Backward::Object
    tmxlite
)

target_link_libraries(main PRIVATE ${LIBS})

file(COPY assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
