cmake_minimum_required(VERSION 4.2)
project(sim-world)

####################
# Define variables #
####################

# Define a variable
set(
    SOURCES
    src/main.cpp
    src/fi.cpp
)

# Define filename variable using GLOB
# file(
#     GLOB
#     SOURCES
#     "src/*.cpp"
# )

##########################
# Define and link static #
##########################

add_library(
    foo_library
    STATIC
    src/foo.cpp
)
target_include_directories(
    foo_library
    PRIVATE # This means only for this target
    # INTERFACE # This means only for targets that link to this target
    # PUBLIC # This means both for this target and targets that link to this target
    ${PROJECT_SOURCE_DIR}/include # we could specify /static but it is nice to see in cpp the include statement specify /static there.
)

##############################
# Define and link executable #
##############################

add_executable(
    ${PROJECT_NAME}
    ${SOURCES}
)
target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    foo_library
)
