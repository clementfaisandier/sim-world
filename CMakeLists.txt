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

##################################
# Define and link static library #
##################################

add_library(
    foo_library
    STATIC # Specify static .a lib
    src/foo.cpp
)
target_include_directories(
    foo_library
    PRIVATE # This means only for this target
    # INTERFACE # This means only for targets that link to this target
    # PUBLIC # This means both for this target and targets that link to this target
    ${PROJECT_SOURCE_DIR}/include # we could specify /static but it is nice to see in cpp the include statement specify /static there.
)

##################################
# Define and link shared library #
##################################

add_library(
    lalo_library
    SHARED # This specifies .so dynamic lib generation
    src/lalo.cpp
)
# add_library(
#     lalo::library
#     ALIAS # Specify a new name/target for this lib
#     lalo_library
# )
target_include_directories(
    lalo_library # Can't use alias target on this function...
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
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
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    lalo_library
)

###################################
# Install execs, libs,... locally #
###################################

# Use `sudo make install` to install.
# Use `sudo xargs rm < install_manifest.txt` to uninstall  

# The default destination is set by a variable called CMAKE_INSTALL_PREFIX
#   which on linux is /usr/local and on windows is C:/Program Files/${PROJECT_NAME}
# Install exec to /usr/local/bin
install(
    TARGETS ${PROJECT_NAME}
    DESTINATION bin
)
install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include
    DESTINATION include 
)
install(
    TARGETS lalo_library # again no alias, not sure why use aliases...
    LIBRARY DESTINATION lib
)
# And all the config files... to etc, man, ...
# install(
#     FILES ${PROJECT_NAME}.conf
#     DESTINATION etc
# )
