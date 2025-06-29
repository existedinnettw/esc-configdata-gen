import os
from conan import ConanFile
from conan.tools.build import can_run


class esc_configdata_genTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        self.requires(self.tested_reference_str)

    def test(self):
        if can_run(self):
            cmd = ["esc-configdata-gen", "-d", "050C08EE1027"]
            self.run(" ".join(cmd), env="conanrun")
