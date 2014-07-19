#include "jconer/util.hpp"
#include <iostream>
#include <assert.h>
using namespace std;
using namespace JCONER;

int main() {
    VarString vs; 
    assert (vs.toString() == "");
    vs.append('a');
    vs.append('b');
    vs.append('c');
    assert (vs.toString() == "abc");
    vs.append("def");
    assert (vs.toString() == "abcdef");
}
