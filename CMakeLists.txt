cmake_minimum_required(VERSION 4.2)
project(sim-world)

# Define a variable
# set(
#     SOURCES
#     src/main.cpp
#     src/fi.cpp
# )

# Define a variable using GLOB
file(
    GLOB
    SOURCES
    "src/*.cpp"
)

# Configure what source we want in what executable
add_executable(
    ${PROJECT_NAME}
    ${SOURCES}
)

# Define some include files for a target
target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
