#ifndef __JCONER_OBJECT_HPP__
#define __JCONER_OBJECT_HPP__

#include "value.hpp"
#include <map>
#include <set>
#include <list>
#include <vector>

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
            value.serialize(*this);
            if (_index != 0) {
                _array_list[_index - 1]->append(_array_list[_index]);
                _array_list.pop_back();
                _index --;
            }
            _array = _array_list[_index];
            return _array_list[_index];
        }

        template<template<class T, class Allocator> class Container, class ValueType>
        void operator&(Container<ValueType, std::allocator<ValueType> >& value) {
            JArray* item = new JArray();
            _index ++;
            _array_list.push_back(item);
            _array = _array_list[_index];
            for(typename Container<ValueType, std::allocator<ValueType> >::iterator iter = value.begin();
                    iter != value.end(); iter ++) {
                *this & *iter;
            }

            _array_list[_index - 1]->append(_array_list[_index]);
            _array_list.pop_back();
            _index --;
            _array = _array_list[_index];
        }

        template<class V>
        void operator&(std::map<std::string,V>& value) {
            JObject* item = new JObject();
            for(typename std::map<std::string, V>::iterator iter = value.begin();
                    iter != value.end(); iter ++) {
                *this & iter->second;
                JValue* val = _array->pop_back();
                item->put(iter->first, val);
            }
            _array->append(item);
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
