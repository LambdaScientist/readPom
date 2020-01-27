

#include <set>
#include <string>
#include <iostream>

#include "rapidxml_utils.hpp"
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

namespace POM {

using namespace rapidxml;

typedef const char* Tag;
struct GetTag{
    GetTag(Tag iTag) {
        tag = iTag;
    }
    auto operator()(xml_node<> *node){
        return node->first_node(tag);
    }
    // template <typename T>
    // T orElse(xml_node<> *node, T defaultValue){
    //     auto result = node->first_node(tag);
    //     return result == nullptr ? defaultValue : *result;
    // }
    private: 
        Tag tag;
};

xml_node<>* operator^(xml_node<> *node, GetTag getTag){
    return node == nullptr ? nullptr : getTag(node); 
}


typedef std::string GroupID;
typedef std::string ArtifactID;
typedef std::string Version;
typedef std::string Scope;
typedef std::string Optional;
typedef std::string Identifier;

GetTag tag_groupid = GetTag("groupId");
GetTag tag_artifactid = GetTag("artifactId");
GetTag tag_version = GetTag("version");
GetTag tag_scope = GetTag("scope");
GetTag tag_optional = GetTag("optional");

GetTag tag_deps = GetTag("dependencies");
GetTag tag_dep = GetTag("dependency");

template<typename T>
Identifier getIdentifier(T t){
    return t.groupID + ":" + t.artifactID + ":" + t.version;
}
struct PackageDetails {
    GroupID groupID;
    ArtifactID artifactID;
    Version version;
    Scope scope;
    Optional optional;
};
struct PackageCompare{
    bool operator()(PackageDetails a, PackageDetails b) { 
        return getIdentifier(a) < getIdentifier(b); 
    };
};

struct Project{
    PackageDetails details;
    std::set<PackageDetails,PackageCompare> dependencies;
};

template <typename T>
T orElse(xml_node<> *node, T orElseValue){
    return node == nullptr ? orElseValue : node->value();
}

template <typename T>
T gather(xml_node<> *node){

}
template <>
inline PackageDetails gather<PackageDetails>(xml_node<> *node){
    PackageDetails pd;
    pd.groupID = orElse(tag_groupid(node),"missing value for groupID");
    pd.artifactID = orElse(tag_artifactid(node),"missing value for artifactID");
    pd.version = orElse(tag_version(node),"missing value for version");
    pd.scope = orElse(tag_scope(node),"missing value for scope");
    pd.optional = orElse(tag_optional(node),"missing value for optional");
    return pd;
}

template <>
inline Project gather<Project>(xml_node<> *node){
    Project p;
    p.details = gather<PackageDetails>(node);
    for (xml_node<> *child = tag_deps(node)->first_node() ; child; child = child->next_sibling()){
        p.dependencies.emplace(gather<PackageDetails>(child));
    }
    return p;
}

inline std::ostream& operator<<( std::ostream &output, const PackageDetails &p ) { 
    output << "" << getIdentifier(p) << "";
    return output;            
}
inline std::ostream& operator<<( std::ostream &output, const Project &p ) { 
    output << "******************************\n";
    output << "* Project: " << p.details << "\n";
    output << "++++++++++++++++++++++++++++++\n";
    output << "+ Dependencies + \n";
    output << "++++++++++++++++++++++++++++++\n";
    for (const auto& i: p.dependencies){
        output << "- " <<  i << "\n";
    }
    output << "******************************\n";
    return output;            
}


}