from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout


class SimWorld(ConanFile):

    name = "sim-world"
    version = "0.0.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def requirements(self):
        self.requires("glfw/3.4")
        self.requires("glew/2.2.0")
        self.requires("glm/1.0.1")

    def build_requirements(self):
        self.tool_requires("cmake/[^4.2]")
        self.tool_requires("pkgconf/[^2.5.1]")
        self.test_requires("doctest/[^2.4.12]")

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
