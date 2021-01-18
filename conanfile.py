from conans import ConanFile, CMake,tools


class sparqlParserBase(ConanFile):
    name = "sparql-parser"
    version = "0.1.0"
    author = "DICE Group <info@dice-research.org>"
    description = "SPARQL parser."
    homepage = "https://github.com/dice-group/sparql-parser"
    url = homepage
    license = "AGPL"
    topics = "SPARQL", "parser", "semantic web"
    settings = "os", "compiler", "build_type", "arch"
    requires="sparql-parser-base/0.2.1@dice-group/stable","sparql-query/0.10@dice-group/stable","rdf-parser/0.12@dice-group/stable","robin-hood-hashing/3.9.1","gtest/1.8.1"
    generators = "cmake", "cmake_find_package", "cmake_paths"
    exports = "LICENSE.txt"
    exports_sources = (
        "CMakeLists.txt",
        "cmake/*",
         "include/*")
    no_copy_source = True

    def package(self):
        cmake = CMake(self)
        cmake.definitions["rdf_parser_BUILD_TESTS"] = "OFF"
        cmake.configure()
        cmake.install()

    def package_id(self):
        self.info.header_only()

    def imports(self):
        self.copy("license*", dst="licenses", folder=True, ignore_case=True)
