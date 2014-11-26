#ifndef __JCONER_DUMP_HPP__
#define __JCONER_DUMP_HPP__

#include "jconer/token.hpp"
#include "jconer/value.hpp"
#include "jconer/stream.hpp"
#include "jconer/parser.hpp"

#include <iostream>

#define DUMP_SORT_KEY 0x01
#define DUMP_PRETTY_PRINT 0x02
#define DUMP_ENSURE_ASCII 0x04
#define DUMP_COMPACT_PRINT 0x08

#define DUMP_INDENT 4

namespace JCONER {

void dump(JValue* value, std::ostream& out, int flag = DUMP_ENSURE_ASCII);
void dumpFile(JValue* value, std::string filename, int flag = DUMP_ENSURE_ASCII);
std::string dumps(JValue* value, int flag = DUMP_ENSURE_ASCII);
void dumps(JValue* value, char** pbuffer, int flag = DUMP_ENSURE_ASCII);

}

#endif
