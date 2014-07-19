#include "jconer/json.hpp"
using namespace JCONER;

int main(int argc, char** argv) {
    PError err;
    //JValue *rst = loads("\"\\u6797\"", err);
    JValue* rst = load(argv[1], err);
    if (rst != NULL) {
        //rst->printout();
        //printf("\n");
        dumpFile(rst, "myjson", DUMP_PRETTY_PRINT | DUMP_SORT_KEY);
        delete rst;
    } else {
        printf("%s", err.text.c_str()); 
    }
}
