#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <string>
#include <iostream>
#include <typeinfo>
#include <memory>
#include <stdexcept>

#include "tinyxml2.h"

using std::string;
using std::cout;
using std::cin;
using std::endl;


class Serializer {
public:
  virtual int save(const string& save_to, tinyxml2::XMLElement* parent_element = nullptr, tinyxml2::XMLDocument* parent_document = nullptr, const char* my_name = nullptr) const = 0;
  virtual int load(const string& load_from, tinyxml2::XMLElement* parent_element = nullptr, tinyxml2::XMLDocument* parent_document = nullptr, const char* my_name = nullptr) = 0;
};


namespace loader{
  using namespace tinyxml2;
  //here defined all posible overloads for loading different types of variables

  static int task(Serializer& var, const char* var_name, const char* var_type_name, XMLElement* parent_element, XMLDocument* the_document) {
    (void)var_name;
    (void)var_type_name;
    var.load("",parent_element,the_document,var_name);
    return 0;
  }

  static int task(double& var, const char* var_name, const char* var_type_name, XMLElement* parent_element, XMLDocument* the_document) {
    (void)the_document;
    (void)var_type_name;
    auto me = parent_element->FirstChildElement(var_name);
    if (me) {
        var = std::atof(me->GetText());
      }
    else var = 0;
    return 0;
  }

}

namespace saver{
  using namespace tinyxml2;
  //here defined all posible overloads for saving different types of variables

  static int task(const Serializer& var, const char* var_name, const char* var_type_name, XMLElement* parent_element, XMLDocument* the_document) {
    (void)var_name;
    (void)var_type_name;
    var.save("",parent_element,the_document,var_name);
    return 0;
  }

    static int task(const double& var, const char* var_name, const char* var_type_name, XMLElement* parent_element, XMLDocument* the_document) {
    auto me =  the_document->NewElement(var_name);
    me->SetText(var);
    me->SetAttribute("typename",var_type_name);
    parent_element->LinkEndChild(me);
    return 0;
  }


}

#define REG_MEMBER(x) task(x, #x, ('_'+std::string(typeid(x).name())).c_str(), root_element, serialiser_document);

#define SAVER(membs) \
int save(const string& save_where, tinyxml2::XMLElement* parent_element = nullptr, tinyxml2::XMLDocument* parent_document = nullptr, const char* my_name = nullptr) const override { \
  using namespace saver; \
  tinyxml2::XMLDocument* serialiser_document = nullptr;\
  std::unique_ptr<tinyxml2::XMLDocument> memory_cleaner(nullptr); \
  tinyxml2::XMLElement* root_element = 0; \
  if(parent_document) {\
    serialiser_document = parent_document;\
    } else {\
    serialiser_document = new tinyxml2::XMLDocument();\
    memory_cleaner.reset(serialiser_document);\
    }\
  if(!parent_element) {  \
    std::string type_name(typeid(*this).name()); \
    root_element = serialiser_document->NewElement(('_'+type_name).c_str());\
    root_element = serialiser_document->LinkEndChild(root_element)->ToElement();\
    } else {\
    std::string type_name(typeid(*this).name()); \
    root_element = serialiser_document->NewElement(('_'+type_name).c_str());\
    root_element = parent_element->LinkEndChild(root_element)->ToElement();\
    }\
  if (my_name) {\
    root_element->SetAttribute("typename",root_element->Name());\
    root_element->SetName(my_name);\
    } \
  membs /*here is the magic*/\
  if(serialiser_document)\
    return serialiser_document->SaveFile(save_where.c_str()); \
    else \
    return 0;\
}

#define LOADER(membs) \
int load(const string& load_from, tinyxml2::XMLElement* parent_element = nullptr, tinyxml2::XMLDocument* parent_document = nullptr, const char* my_name = nullptr) override { \
  using namespace loader; \
  tinyxml2::XMLDocument* serialiser_document = nullptr;\
  std::unique_ptr<tinyxml2::XMLDocument> memory_cleaner(nullptr); \
  tinyxml2::XMLElement* root_element = 0; \
  if(parent_document) {\
    serialiser_document = parent_document;\
    } else {\
    serialiser_document = new tinyxml2::XMLDocument();\
    memory_cleaner.reset(serialiser_document);\
    }\
  if(!parent_element) {  \
    if(serialiser_document->LoadFile(load_from.c_str())!=XML_NO_ERROR) {\
	  throw std::runtime_error("File: "+load_from+"can't be opened or incorrect.");\
	} \
    root_element = serialiser_document->RootElement();\
  } else {\
    if(!my_name) { \
      root_element = parent_element;\
    } else {\
      root_element = parent_element->FirstChildElement(my_name);\
	  if(!root_element) {\
		root_element = parent_element;\
	  }\
    }\
  }\
  membs \
  return 0; \
}

#define SAVE(var) SAVER(var) LOADER(var)


#endif //SERIALIZER_H
