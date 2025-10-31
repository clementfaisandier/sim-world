cmake_minimum_required(VERSION 4.2)

project(sim-world)

file(
    GLOB
    SOURCES
    "src/*.cpp"
)

add_executable(
    ${PROJECT_NAME}
    ${SOURCES}
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
