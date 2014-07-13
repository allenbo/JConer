#include "stream.hpp"
#include "token.hpp"
#include <iostream>
using namespace JCONER;
using namespace std;

int main(int argc, char** argv) {
    FileIStream stream(argv[1]);
    while(true) {
        Token t = stream.getNextToken();
        if (t.type() == TT_END) break;
    }
}
