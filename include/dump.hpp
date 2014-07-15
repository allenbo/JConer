#ifndef __JCONER_DUMP_HPP__
#define __JCONER_DUMP_HPP__

#include "token.hpp"
#include "value.hpp"
#include "stream.hpp"
#include "parser.hpp"

#include <iostream>

#define DUMP_SORT_KEY 0x01
#define DUMP_PRETTY_PRINT 0x02
#define DUMP_ENSURE_ASCII 0x04

#define DUMP_INDENT 4

namespace JCONER {

void dump(JValue* value, std::ostream& out, int flag = DUMP_ENSURE_ASCII);
void dumpFile(JValue* value, std::string filename, int flag = DUMP_ENSURE_ASCII);
void dumps(JValue* value, char** pbuffer, int flag = DUMP_ENSURE_ASCII);

}

#endif
