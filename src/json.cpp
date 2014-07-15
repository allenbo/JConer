#include "json.hpp"

namespace JCONER {

JValue* load(std::string filename) {
    FileIStream fin(filename);
    Parser parser(fin);
    return parser.parse();
}

JValue* load(const char* filename) {
    std::string str_filename(filename);
    return load(str_filename);
}

}
