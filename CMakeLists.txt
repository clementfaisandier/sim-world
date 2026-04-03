cmake_minimum_required(VERSION 4.2)
project(sim-world)

####################
# Define variables #
####################

# Define filename variable using GLOB
file(GLOB SOURCES "src/*.cpp" )

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

add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    GLEW::GLEW
    glfw
    glm::glm
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
