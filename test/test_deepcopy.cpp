#include "jconer/json.hpp"
#include <iostream>

using namespace JCONER;

int main(int argc, char** argv) {
    PError err;
    JValue* rst = load(argv[1], err);
    if (rst != NULL) {
        JValue* copy = rst->deepcopy();
        dump(copy, std::cout);
        delete copy;
        delete rst;
    }
}
