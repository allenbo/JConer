#include "jconer/json.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <cassert>

using namespace JCONER;

class Simple {
    public:
        Simple() : int_value(0), string_value("") { }
        Simple(int i, std::string s): int_value(i), string_value(s) {}
        Simple(const Simple& s): int_value(s.int_value), string_value(s.string_value) {}

        template<class Serializer>
            void serialize(Serializer& serializer) {
                serializer & int_value;      
                serializer & string_value;
            }
        friend std::ostream& operator<<(std::ostream& out, const Simple& s) {
            out << s.int_value << " " << s.string_value;
            return out;
        }
        friend bool operator==(const Simple& lhs, const Simple& rhs);
    private:
        int int_value;
        std::string string_value;
};

bool operator==(const Simple& lhs, const Simple& rhs) {
    return lhs.int_value == rhs.int_value && lhs.string_value == rhs.string_value;
}

bool operator!=(const Simple& lhs, const Simple& rhs) {
    return !(lhs == rhs);
}


class Complex {
    public:
        Complex():int_value(0), string_value("") {
            vector_float_value.clear();
        }

        Complex(int i, std::string s): int_value(i), string_value(s) {
            for(int i = 0; i < 10; i ++ ) {
                vector_float_value.push_back(i * 1.2);
            }
        }

        template<class Serializer>
            void serialize(Serializer& serializer) {
                serializer & int_value;
                serializer & string_value;
                serializer & vector_float_value;
            }
        friend std::ostream& operator<<(std::ostream& out, const Complex& c) {
            out << c.int_value << " " << c.string_value << " [ ";
            for(int i = 0; i < 10; i ++ ) {
                out << c.vector_float_value[i];
                if (i != 10 - 1) {
                    out << " ,";
                }
            }
            out << " ]";
            return out;
        }
        friend bool operator==(const Complex& lhs, const Complex& rhs);
    private:
        int int_value;
        std::string string_value;
        std::vector<float> vector_float_value;
};

bool operator==(const Complex& lhs, const Complex& rhs) {
    if (lhs.int_value != rhs.int_value) return false;
    if (lhs.string_value != rhs.string_value) return false;
    if (lhs.vector_float_value.size() != rhs.vector_float_value.size()) return false;
    for(int i = 0; i < lhs.vector_float_value.size(); i ++) {
        if (lhs.vector_float_value[i] != rhs.vector_float_value[i]) return false;
    }
    return true;
}

bool operator!=(const Complex& lhs, const Complex& rhs) {
    return !(lhs == rhs);
}

void test_primitives() {
    // out serializer
    OutSerializer sout;
    sout & 4;
    sout & 1.1f;
    sout & "string";
    JValue* content = sout.get_content();
    dump(content, std::cout, DUMP_ENSURE_ASCII);
    std::cout << std::endl << "Suppose to print out [" << 4 << ", " <<  1.1f << ", string ]" << std::endl;

    // in serializer
    InSerializer sin(content);
    int i;
    float f;
    std::string s;

    sin & i;
    sin & f;
    sin & s;

    assert (i == 4 && f == 1.1f && s == "string");

}

void test_vector_primitive() {
    // out serializer 
    OutSerializer sout;
    std::vector<int> int_vec(10);
    for(int i = 0; i < 10; i ++ ) {
        int_vec[i] = i;
    }

    sout & int_vec;
    JValue* content = sout.get_content();
    dump(content, std::cout, DUMP_ENSURE_ASCII);

    std::cout << std::endl << "Suppose to print out [ [";
    for(int i = 0; i < 10; i ++ ) {
        std::cout << i;
        if (i != 10 - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "] ]" << std::endl;

    // in serializer
    InSerializer sin(content);
    std::vector<int> another_int_vec;

    sin & another_int_vec;
    assert (another_int_vec.size() == 10);
    for(int i = 0; i < 10; i ++ ) {
        assert(another_int_vec[i] == i);
    }
}

void  test_pair() {
    // out serializer
    OutSerializer sout;
    std::pair<int, double> p = std::make_pair(20, 10.2);

    sout & p;
    JValue* content = sout.get_content();
    dump(content, std::cout, DUMP_ENSURE_ASCII);

    std::cout << std::endl << "Suppose to print out [ [";
    std::cout << "20 , 10.2 ] ]" << std::endl;

    // in serializer
    InSerializer sin(content);
    std::pair<int, double> anp;
    sin & anp;
    assert(anp.first == p.first && anp.second == p.second);
}

void test_map() {
    // out serializer
    OutSerializer sout;
    std::map<int, std::string> m;
    for(int i = 0; i < 10; i ++) {
        m[i] = "value";
    }

    sout & m;
    JValue* content = sout.get_content();
    dump(content, std::cout, DUMP_ENSURE_ASCII);

    std::cout << std::endl << "Suppose to print out [ [";
    for(int i = 0; i < 10; i ++ ) {
        std::cout << "[ " << i << ", value ]";
        if (i != 9 ) {
            std::cout << ", ";
        }
    }
    std::cout << "] ]" << std::endl;

    // in seralizer
    InSerializer sin(content);
    std::map<int, std::string> anm;
    sin & anm;

    assert(anm.size() == 10);
    for(int i = 0; i < 10; i ++) {
        assert(anm.count(i) == 1);
        assert(anm[i] == "value");
    }
}

void test_simple() {
    // out serializer 
    OutSerializer sout;
    Simple s(10, "John");

    sout & s;
    JValue* content = sout.get_content();
    dump(content, std::cout, DUMP_ENSURE_ASCII);

    std::cout << std::endl << "Suppose to print out [ [";
    std::cout << s << " ] ]" << std::endl;

    // in serializer
    InSerializer sin(content);
    Simple ans;
    sin & ans;

    assert(ans == s);
}

void test_complex() {
    // out serializer
    OutSerializer sout;
    Complex c(10, "John");

    sout & c;
    JValue* content = sout.get_content();
    dump(content, std::cout, DUMP_ENSURE_ASCII);

    std::cout << std::endl << "Suppose to print out [ [";
    std::cout << c << " ] ]" << std::endl;

    // in serializer
    InSerializer sin(content);
    Complex anc;

    sin & anc;

    assert(anc == c);
}

int main() {
    test_primitives();
    test_vector_primitive();

    test_pair();
    test_map();

    test_simple();
    test_complex();
}
