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

        template<class Serializer>
        void toJArray(Serializer& serializer) {
            serializer & _state;
            serializer & _city;
        }
    
    private:
        std::string _state;
        std::string _city;
};

class Test {
    public:
        Test(int x, int y)
            :_x(x), _y(y), _loc("CA", "San Jose") {
            _name = "allenbo";
            _salary = 7549.8;
            _male = true;
        }

        template<class Serializer>
        void toJArray(Serializer& serializer) {
            serializer & _x;
            serializer & _y;
            serializer & _name;
            serializer & _salary;
            serializer & _male;
            serializer & _loc;
        }
    private:
        int _x;
        int _y;
        std::string _name;
        double _salary;
        bool _male;
        Location _loc;
};

int main() {
    Test t(1, 2);
    ArraySerializer serializer;
    JValue* arr_item = serializer & t;
    dump(arr_item, std::cout); 
    delete arr_item;
}
