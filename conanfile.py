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
    requires="sparql-parser-base/0.1.1@dice-group/stable","sparql-queryGraph/0.9@dice-group/stable"
    generators = "cmake"
    exports = "LICENSE.txt"
    exports_sources = (
        "CMakeLists.txt",
        "cmake/*")
    no_copy_source = True

    def package(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.install()
        self.copy("*.a", dst="lib", keep_path=False)

    def imports(self):
        self.copy("license*", dst="licenses", folder=True, ignore_case=True)
