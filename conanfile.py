from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps


class esc_configdata_genRecipe(ConanFile):
    name = "esc-configdata-gen"
    version = "0.1"
    package_type = "application"

    # Optional metadata
    license = "MPL-2.0"
    author = "insleker <bkinnightskytw@gmail.com>"
    url = "https://github.com/existedinnettw/esc-configdata-gen"
    description = "ESC SII configdata generation"
    topics = ("SII", "EtherCAT")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*", "tests/*"

    def requirements(self):
        self.requires("argparse/[>=3.2 <4]")
        self.requires("magic_enum/[>=0.9.7 <1]")
        self.test_requires("gtest/[>=1.16.0 <2]")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
