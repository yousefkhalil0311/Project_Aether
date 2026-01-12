# specify minimum CMake Version
cmake_minimum_required(VERSION 3.20.0)

# Locate Zephyr source
find_package(
    Zephyr
    REQUIRED
    HINTS
    ../../../zephyr
)

# Project name
project(
    Project_Aether
    VERSION 1.0
    DESCRIPTION "Embedded Connectivity Platform"
    LANGUAGES C
)

# Static libraries
add_library(
    my_lib
    STATIC
    src/my_lib.c
)

# Set include directories for libraries
target_include_directories(
    my_lib
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Create executable target (Zephyr takes care of this. Target name = app)
# add_executable(
#     ${PROJECT_NAME}
#     src/main.c
# )

# Locate source code for application
target_sources(
    app
    PRIVATE
    src/main.c
)

# Link libraries to executable target
target_link_libraries(
    app
    PRIVATE
    my_lib
)