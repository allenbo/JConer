#include "parser.hpp"
#include "stream.hpp"
#include "value.hpp"
#include "error.hpp"

using namespace JCONER;

int main(int argc, char** argv) {
    PError err;
    //JValue *rst = loads("\"\\u6797\"", err);
    JValue* rst = load(argv[1], err);
    if (rst != NULL) {
        rst->printout();
        printf("\n");
        delete rst;
    } else {
        printf("%s", err.text.c_str()); 
    }
}
