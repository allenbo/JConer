#ifndef __JCONER_JSON_HPP__
#define __JCONER_JSON_HPP__

#include "value.hpp"
#include "stream.hpp"
#include "parser.hpp"
#include "token.hpp"

#include <iostream>

namespace JCONER {

//void dump(JValue* value, std::ostream& out);
//void dump(JValue* value, std::ostream& out, PrintProperty& property);

JValue* load(std::string filename);
JValue* load(const char* filename);
//JValue* loads(const char* buffer);

}
#endif
