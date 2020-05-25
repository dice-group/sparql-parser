//
// Created by fakhr on 25.05.20.
//

#ifndef SPARQL_PARSER_PROPERTYPATH_HPP
#define SPARQL_PARSER_PROPERTYPATH_HPP

#include <Dice/rdf_parser/RDF/Term.hpp>

class PropertySetElement
{
private:
    bool isInversed;

public:
    PropertySetElement(bool isInversed):isInversed(isInversed){};
};

class IriPropertySetElement:public PropertySetElement
{
private:
    rdf_parser::store::rdf::URIRef iri;
    bool isInversed;

public:
    IriPropertySetElement(rdf_parser::store::rdf::URIRef iri,bool isInversed):PropertySetElement(isInversed),iri(iri){};
};

class APropertySetElement:public PropertySetElement
{

public:
    APropertySetElement(bool isInversed):PropertySetElement(isInversed){};
};



#endif //SPARQL_PARSER_PROPERTYPATH_HPP
