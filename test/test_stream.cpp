#include "stream.hpp"
#include "token.hpp"
#include <iostream>
using namespace JCONER;
using namespace std;

int main(int argc, char** argv) {
    ifstream in(argv[1]);
    IStream stream(in);
    while(true) {
        Token t = stream.getNextToken();
        if (t.type() == TT_END) break;
    }
}
