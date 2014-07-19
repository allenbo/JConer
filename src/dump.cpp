#include "jconer/util.hpp"
#include "jconer/dump.hpp"
#include <fstream>
#include <sstream>

namespace JCONER {

static void dumpInner(JValue* value, std::ostream& out, int flag, int indent) {
    bool pretty_print = flag & DUMP_PRETTY_PRINT;
    bool sort_key = flag & DUMP_SORT_KEY;
    bool ensure_ascii = flag & DUMP_ENSURE_ASCII;
    
    switch(value->type()) {
        case VT_INTEGER:
            out << ((JInt*)value)->getValue();
            break;
        case VT_REAL:
            out << ((JReal*)value)->getValue();
            break;
        case VT_TRUE:
            out << "true";
            break;
        case VT_FALSE:
            out << "false";
            break;
        case VT_NULL:
            out << "null";
            break;
        case VT_STRING:
            if (ensure_ascii) {
                out << '"' << ((JString*)value)->getAsciiValue() << '"';
            } else {
                out << '"' << ((JString*)value)->getValue() << '"';
            }
            break;
        case VT_OBJECT:
            {
            JObject* ovalue = (JObject*) value;
            std::vector<std::string> keys = ovalue->getKeys();
            if (sort_key) {
                std::sort(keys.begin(), keys.end());
            }
            
            out << "{";
            
            for (int i = 0; i < keys.size(); i ++ ) {
                if (pretty_print) {
                    out << std::endl;
                    std::string indent_string(indent, ' ');
                    out << indent_string;
                }
                else out << " ";

                out << "\"" << keys[i] << "\": ";
                dumpInner(ovalue->get(keys[i]), out, flag, indent + DUMP_INDENT);
                if (i != keys.size() - 1) {
                    out << ",";
                }
            }
            if (pretty_print) {
                out << std::endl;
                std::string indent_string(indent - DUMP_INDENT, ' ');
                out << indent_string;
            }
            else out << " ";
            
            out << "}";
            }
            break;
        case VT_ARRAY:
            {
            JArray* avalue = (JArray*) value;
            out << "[";
            for (int i = 0; i < avalue->size(); i ++ ) {
                if (pretty_print) {
                    out << std::endl;
                    std::string indent_string(indent, ' ');
                    out << indent_string;
                } else out << " ";

                dumpInner(avalue->get(i), out, flag, indent + DUMP_INDENT);
                if (i != avalue->size() - 1) {
                    out  << ",";
                }
            }
            if (pretty_print) {
                out << std::endl;
                std::string indent_string(indent - DUMP_INDENT, ' ');
                out << indent_string;
            }
            else out << " ";
            
            out << "]";
            }
            break;
    }
}

void dump(JValue* value, std::ostream& out, int flag) {
    dumpInner(value, out, flag, DUMP_INDENT);
}

void dumpFile(JValue* value, std::string filename, int flag) {
    std::ofstream fout(filename.c_str()); 
    dump(value, fout, flag);
    fout.close();
}

void dumps(JValue* value, char** pbuffer, int flag) {
    std::stringstream ssout;
    dump(value, ssout, flag);
    std::string jsonString = ssout.str();
    int strSize = jsonString.size();
    MALLOC(char, *pbuffer, strSize + 1);
    strncpy(*pbuffer, jsonString.c_str(), strSize + 1);
}

}
