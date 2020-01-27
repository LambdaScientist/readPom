


#include <iostream>
#include <fstream>
#include <vector>

#include <ReadPom.hpp>

int main(int argc, char** argv){
  using namespace POM;
  file<> xmlfile(argv[1]);
  xml_document<> doc;    
  doc.parse<0>(xmlfile.data()); 

  rapidxml::xml_node<> *project = doc.first_node();
  std::cout << gather<Project>(project) << "\n";

}