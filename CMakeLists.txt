cmake_minimum_required(VERSION 4.2)
project(sim-world)

####################
# Define variables #
####################

# Define filename variable using GLOB
file(
    GLOB
    SOURCES
    "src/*.cpp"
)

##################################
# Define and link shared library #
##################################

# OpenGL related libraries
find_package(
    OpenGL
    REQUIRED
    COMPONENTS OpenGL
)
find_package(
    GLEW
    REQUIRED
)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
find_package(glm CONFIG REQUIRED)

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
    ${GLEW_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    ${GLEW_LIBRARIES}
    ${GLFW_LIBRARIES}
    glm::glm
)
