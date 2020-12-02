//
// Created by fakhr on 25.05.20.
//

#ifndef SPARQL_PARSER_PROPERTYPATH_HPP
#define SPARQL_PARSER_PROPERTYPATH_HPP

#include <Dice/rdf_parser/RDF/Term.hpp>

#include "PathMode.hpp"

#include "PathAlternative.hpp"

namespace SparqlParser::internal {
    class PropertySetElement {
    private:
        bool isInversed;
        PathMode pathMode;

    public:
        PropertySetElement(bool isInversed, PathMode pathMode) : isInversed(isInversed), pathMode(pathMode) {};

        void setIsInversed(bool isInversed) {
            this->isInversed = isInversed;
        }

        void setpathMode(PathMode pathMode) {
            this->pathMode = pathMode;
        }
    };


    class SinglePropertySetElement : public PropertySetElement {
    public:
        SinglePropertySetElement(bool isInversed, PathMode pathMode) : PropertySetElement(isInversed, pathMode) {};
    };


    class IriPropertySetElement : public SinglePropertySetElement {
    private:
        rdf_parser::store::rdf::URIRef iri;

    public:
        IriPropertySetElement(rdf_parser::store::rdf::URIRef iri, bool isInversed, PathMode pathMode)
                : SinglePropertySetElement(isInversed, pathMode), iri(iri) {};

        IriPropertySetElement(rdf_parser::store::rdf::URIRef iri, bool isInversed) : SinglePropertySetElement(
                isInversed, PathMode::None), iri(iri) {};

        IriPropertySetElement(rdf_parser::store::rdf::URIRef iri) : SinglePropertySetElement(false, PathMode::None),
                                                                    iri(iri) {};

        IriPropertySetElement(rdf_parser::store::rdf::URIRef iri, PathMode pathMode) : SinglePropertySetElement(false,
                                                                                                                pathMode),
                                                                                       iri(iri) {};
    };


    class APropertySetElement : public SinglePropertySetElement {

    public:
        APropertySetElement(bool isInversed, PathMode pathMode) : SinglePropertySetElement(isInversed, pathMode) {};

        APropertySetElement(PathMode pathMode) : SinglePropertySetElement(false, pathMode) {};

        APropertySetElement(bool isInversed) : SinglePropertySetElement(isInversed, PathMode::None) {};
    };


    class Path : public PropertySetElement {
    private:
        PathAlternative alternative;

    };

}

//class PathNegatedPropertySet :public PropertySetElement
//{
//private:
//    std::vector<SinglePropertySetElement>
//};


#endif //SPARQL_PARSER_PROPERTYPATH_HPP
