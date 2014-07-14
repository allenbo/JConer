#ifndef __JCONER_NODE_HPP__
#define __JCONER_NODE_HPP__

#include "logging.hpp"
#include <vector>

namespace JCONER {

enum NodeType {
    NT_MESSAGE = 0,
    NT_VALUE,
    NT_ARRAY,
    NT_OBJECT,
    NT_PRIMITIVE,
    NT_ARRAY_OPEN_BRACE,
    NT_ARRAY_CLOSE_BRACE,
    NT_VALUES,
    NT_OBJECT_OPEN_BRACE,
    NT_OBJECT_CLOSE_BRACE,
    NT_PAIRS,
    NT_PAIR,
    NT_COMMA,
    NT_COLON,
    NT_STRING,
    NT_INTEGER,
    NT_REAL,
    NT_TRUE,
    NT_FALSE,
    NT_NULL
};

class Node {
    CLASS_MAKE_LOGGER
    public:
        Node();
        inline int nch() const { return _children.size(); }
    private:
        NodeType _type;
        int _lineno;
        int _col;

        std::vector<Node*> _children;
        Node* _parent;
};

}

#endif
