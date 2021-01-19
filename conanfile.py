from conans import ConanFile, CMake,tools


class sparqlParserBase(ConanFile):
    name = "sparql-parser"
    author = "DICE Group <info@dice-research.org>"
    description = "SPARQL parser."
    homepage = "https://github.com/dice-group/sparql-parser"
    url = homepage
    license = "AGPL"
    topics = "SPARQL", "parser", "semantic web"
    settings = "os", "compiler", "build_type", "arch"
    requires="sparql-parser-base/0.2.2@dice-group/stable","sparql-query/0.10@dice-group/stable","rdf-parser/0.12@dice-group/stable","robin-hood-hashing/3.9.1","gtest/1.8.1"
    options = {'with_tests': [False, True]}
    default_options = {'with_tests': False}
    generators = "cmake", "cmake_find_package", "cmake_paths"
    exports = "LICENSE.txt"
    exports_sources = (
        "CMakeLists.txt",
        "cmake/*",
         "include/*")
    no_copy_source = True

    def requirements(self):
        if self.options.with_tests:
            self.requires("gtest/1.8.1")

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
