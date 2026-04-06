cmake_minimum_required(VERSION 4.2)
project(sim-world)
set(PROJECT_LIB ${PROJECT_NAME}-lib)
include(CTest)

####################
# Define variables #
####################

# Define filename variable using GLOB
file(GLOB SOURCES "src/*.cpp" )
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")    

##################################
# Define and link shared library #
##################################

# OpenGL related libraries
find_package(GLEW REQUIRED GL)
find_package(glfw3 REQUIRED GLFW)
find_package(glm CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

##############################
# Define and link executable #
##############################

# First building a library to enable testing.

add_library(
    ${PROJECT_LIB}
    STATIC
    ${SOURCES}
)

# Link libraries
target_link_libraries(
    ${PROJECT_LIB}
    PRIVATE
    OpenGL::GL
    GLEW::GLEW
    glfw
    glm::glm
)

target_include_directories(
    ${PROJECT_LIB}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/shaders
)

# Then create the executable.

add_executable(
    ${PROJECT_NAME}
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    ${PROJECT_LIB}
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/shaders
)

###########
# Testing #
###########

if(BUILD_TESTING)
    set(TEST_NAME ${PROJECT_NAME}-tests)

    file(GLOB TEST_SOURCES "tests/*.cpp")
    add_executable(${TEST_NAME} ${TEST_SOURCES})

    find_package(doctest REQUIRED)
    target_link_libraries(
        ${TEST_NAME}
        PRIVATE
        ${PROJECT_LIB}
        doctest::doctest
    )
    target_include_directories(
        ${TEST_NAME}
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/shaders
    )

    include(doctest)
    doctest_discover_tests(${TEST_NAME})
endif()
