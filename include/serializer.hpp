#ifndef __JCONER_OBJECT_HPP__
#define __JCONER_OBJECT_HPP__

#include "value.hpp"

namespace JCONER {

class ArraySerializer {
    public:
        ArraySerializer() {
            _index = -1;
            _array = NULL;
        }

        template<class Type>
        JValue* operator&(Type& value) {
            JArray* item = new JArray();
            _index ++;
            _array_list.push_back(item);
            _array = _array_list[_index];
            value.toJArray(*this); 
            if (_index != 0) {
                _array_list[_index - 1]->append(_array_list[_index]);
                _array_list.pop_back();
                _index --;
            }
            return _array_list[0];
        }

        void operator&(const int value) {
            _array->append(value);
        }

        void operator&(const long value) {
            _array->append(value);
        }

        void operator&(const std::string value) {
            _array->append(value);
        }

        void operator&(const bool value) {
            _array->append(value);
        }

        void operator&(const char* value) {
            _array->append(value);
        }

        void operator&(const double value) {
            _array->append(value);
        }

    private:
        std::vector<JArray*> _array_list;
        int _index;
        JArray* _array;
};
}
#endif
