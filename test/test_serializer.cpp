#include "jconer/json.hpp"
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
        friend bool operator==(const Location & lhs, const Location& rhs);
        friend std::ostream& operator<<(std::ostream& out, const Location& loc);

    private:
        std::string _state;
        std::string _city;
};

bool operator==(const Location &lhs, const Location& rhs) {
    if (lhs._state != rhs._state) return false;
    if (lhs._city != rhs._city) return false;
    return true;
}

std::ostream& operator<<(std::ostream& out, const Location& loc) {
    out << "[" << loc._state << " " << loc._city << "]";
    return out;
}

class Inner {
    public:
        Inner(int __1, int __2)
            :_1(__1), _2(__2) {}

        template<class Serializer>
        void serialize(Serializer& serializer) {
            serializer & _1;
            serializer & _2;
        }
        friend std::ostream& operator<<(std::ostream& out, const Inner& inner);
    private:
        int _1, _2;
};

std::ostream& operator<<(std::ostream& out, const Inner& inner) {
    out << inner._1 << " " << inner._2;
    return out;
}

class NestedInfo {
    public:
        NestedInfo(int __1, int __2, int __3)
            :_1(__1), _2(__2, __3) {}

        template<class Serializer>
        void serialize(Serializer& serializer) {
            serializer & _1;
            serializer & _2;
        }
        friend std::ostream& operator<<(std::ostream& out, const NestedInfo& info);
    private:
        int _1;
        Inner _2;
};
std::ostream& operator<<(std::ostream& out, const NestedInfo& info) {
    out << info._1 << " " << info._2;
    return out;
}

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

        Person(int x, int y, std::string name, double salary, bool male)
            :_x(x), _y(y), _name(name), _salary(salary), _male(male), _nonsense(6, 7, 8) {
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

        bool operator==(const Person& other) {
            if (_x != other._x) return false;
            if (_y != other._y) return false;
            if (_name != other._name) return false;
            if (_salary != other._salary) return false;
            if (_male != other._male) return false;
            //if (_home != other._home) return false;
            //if (_work != other._work) return false;
            return true;
        }
        friend std::ostream& operator<<(std::ostream& out, const Person & t);
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

std::ostream& operator<<(std::ostream& out, const Person & t) {
    out << t._x << " " << t._y << " " << t._name << " " << t._salary << " " << t._male << " " << t._home << " " << t._work << " ";
    out << "[";
    for (int i = 0; i < t._languages.size(); i ++ ) {
        out << t._languages[i] << " ";
    }
    out <<"] ";
    out << "{";
    for (std::map<std::string, int>::const_iterator iter = t._scores.begin(); iter != t._scores.end(); iter ++) {
        out << iter->first << ":" << iter->second << " ";
    }
    out << "} ";
    for (std::map<std::string, Location>::const_iterator iter = t._maps.begin(); iter != t._maps.end(); iter ++) {
        out << iter->first << ":" << iter->second << " ";
    }
    out << "} ";

    out << t._nonsense;

    return out;
}

int main() {
    Person t(1, 2);
    OutSerializer sout;
    JValue* arr_item = sout & t;
    dump(arr_item, std::cout, DUMP_PRETTY_PRINT | DUMP_ENSURE_ASCII);
    std::cout << std::endl;
    InSerializer sin((JArray*)arr_item);
    Person another(3, 4, "another name", 1.1, false);
    sin & another;
    //std::cout << "first " << t << std::endl;
    //std::cout << "another " << another << std::endl;
    delete arr_item;

    arr_item = sout & another;
    dump(arr_item, std::cout, DUMP_PRETTY_PRINT | DUMP_ENSURE_ASCII);
    std::cout << std::endl;
    delete arr_item;
}
