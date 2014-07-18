#include "serializer.hpp"
#include "dump.hpp"
#include <iostream>

using namespace JCONER;

class Location {
    public:
        Location(std::string state, std::string city) {
            _state = state;
            _city = city;
        }

        Location(const Location& other ) {
            _state = other._state;
            _city = other._city;
        }

        Location(){
            _state = "";
            _city = "";
        }

        template<class Serializer>
        void serialize(Serializer& serializer) {
            serializer & _state;
            serializer & _city;
        }

    private:
        std::string _state;
        std::string _city;
};

class Inner {
    public:
        Inner(int __1, int __2)
            :_1(__1), _2(__2) {}

        template<class Serializer>
        void serialize(Serializer& serializer) {
            serializer & _1;
            serializer & _2;
        }
    private:
        int _1, _2;
};

class NestedInfo {
    public:
        NestedInfo(int __1, int __2, int __3)
            :_1(__1), _2(__2, __3) {}

        template<class Serializer>
        void serialize(Serializer& serializer) {
            serializer & _1;
            serializer & _2;
        }
    private:
        int _1;
        Inner _2;
};

class Person {
    public:
        Person(int x, int y)
            :_x(x), _y(y), _home("NY", "New York City"), _work("CA", "San Jose"), _nonsense(3, 4, 5){
            _name = "allenbo";
            _salary = 7549.8;
            _male = true;
            _languages.push_back("C++");
            _languages.push_back("Java");
            _languages.push_back("Python");
            _languages.push_back("Javascript");

            _scores["Operating System"] = 90;
            _scores["Algorithm"] = 80;

            _maps["home"] = _home;
            _maps["work"] = _work;
        }

        template<class Serializer>
        void serialize(Serializer& serializer) {
            serializer & _x;
            serializer & _y;
            serializer & _name;
            serializer & _salary;
            serializer & _male;
            serializer & _home;
            serializer & _work;
            serializer & _languages;
            serializer & _scores;
            serializer & _maps;
            serializer & _nonsense;
        }
    private:
        int _x;
        int _y;
        std::string _name;
        double _salary;
        bool _male;
        Location _home;
        Location _work;
        std::vector<std::string> _languages;
        std::map<std::string, int> _scores;
        std::map<std::string, Location> _maps;

        NestedInfo _nonsense;
};

int main() {
    Person t(1, 2);
    OutSerializer sout;
    JValue* arr_item = sout & t;
    dump(arr_item, std::cout, DUMP_PRETTY_PRINT | DUMP_ENSURE_ASCII);
    std::cout << std::endl;
    InSerializer sin((JArray*)arr_item);
    Person another(3, 4);
    sin & another;
    delete arr_item;
}
