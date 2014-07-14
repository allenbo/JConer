#include "parser.hpp"
#include "stream.hpp"
#include "value.hpp"

using namespace JCONER;

int main(int argc, char** argv) {
    FileIStream fin(argv[1]);
    Parser parser(fin);
    JValue *rst = parser.parse();
    rst->printout();
    printf("\n");
}
