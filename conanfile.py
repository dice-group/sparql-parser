import re, os
from conans.tools import load
from conans import ConanFile, CMake, tools

class sparqlParserBase(ConanFile):
    name = "sparql-parser"
    author = "DICE Group <info@dice-research.org>"
    description = "SPARQL parser."
    homepage = "https://github.com/dice-group/sparql-parser"
    url = homepage
    license = "AGPL"
    topics = "SPARQL", "parser", "semantic web"
    settings = "os", "compiler", "build_type", "arch"
    requires="sparql-parser-base/0.2.2@dice-group/stable","sparql-query/0.11.0@dice-group/rc1","rdf-parser/0.13@dice-group/stable","robin-hood-hashing/3.9.1"
    options = {'with_tests': [False, True]}
    default_options = {'with_tests': False}
    generators = "cmake", "cmake_find_package", "cmake_paths"
    exports = "LICENSE.txt"
    exports_sources = (
        "CMakeLists.txt",
        "cmake/*",
         "include/*")
    no_copy_source = True


    def set_version(self):
        if not hasattr(self, 'version') or self.version is None:
            cmake_file = load(os.path.join(self.recipe_folder, "CMakeLists.txt"))
            self.version = re.search("project\(sparql-parser VERSION (.*)\)", cmake_file).group(1)

    def package(self):
        cmake = CMake(self)
        if self.options.with_tests:
            cmake.definitions["SPARQL_PARSER_BUILD_TESTS"] = "ON"
        cmake.configure()
        cmake.install()

    def package_id(self):
        self.info.header_only()

    def imports(self):
        self.copy("license*", dst="licenses", folder=True, ignore_case=True)
