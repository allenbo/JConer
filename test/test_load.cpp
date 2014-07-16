#include "parser.hpp"
#include "stream.hpp"
#include "value.hpp"

using namespace JCONER;

int main(int argc, char** argv) {
    JValue *rst = loads("[12]");
    rst->printout();
    delete rst;
    printf("\n");
}
