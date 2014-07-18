#ifndef __JCONER_OBJECT_HPP__
#define __JCONER_OBJECT_HPP__

#include "value.hpp"
#include <map>
#include <set>
#include <list>
#include <vector>

namespace JCONER {

class OutSerializer {
    public:
        OutSerializer() {
            _array = NULL;
        }

        template<class Type>
        JValue* operator&(Type& value) {
            JArray* item = new JArray();
            JArray* cur_array = _array;

            if (cur_array != NULL)
                _array->append(item);

            _array = item;
            value.serialize(*this);

            if (cur_array != NULL)
                _array = cur_array;

            return _array;
        }

        template<template<class T, class Allocator> class Container, class ValueType>
        void operator&(Container<ValueType, std::allocator<ValueType> >& value) {
            JArray* item = new JArray();
            JArray* cur_array = _array;
            _array->append(item);

            _array = item;
            for(typename Container<ValueType, std::allocator<ValueType> >::iterator iter = value.begin();
                    iter != value.end(); iter ++) {
                *this & *iter;
            }
            _array = cur_array;
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
        JArray* _array;
};


class InSerializer {
    public:
        InSerializer(JArray* instance) {
            _instance = instance;
            _array = NULL;
            _index = 0;
        }

        template<class Type>
        void operator&(Type& value) {
            JArray* cur_array = _array;
            int cur_index = _index;
            
            if (cur_array != NULL) {
                _array = (JArray*)(cur_array->get(_index));
                _index = 0;
            } else {
                _array = _instance;
            }
            value.serialize(*this);

            _array = cur_array;
            _index = cur_index + 1;
        }

        template<template<class T, class Allocator> class Container, class ValueType>
        void operator&(Container<ValueType, std::allocator<ValueType> >& value) {
            JArray* cur_array = _array;
            int cur_index = _index;

            _array = (JArray*)(cur_array->get(_index));
            _index = 0;
            for(int i = 0; i < _array->size(); i ++) {
                ValueType tmp;
                value.push_back(tmp);
            }

            for(typename Container<ValueType, std::allocator<ValueType> >::iterator iter = value.begin();
                    iter != value.end(); iter ++) {
                *this & *iter;
            }
            _array = cur_array;
            _index = cur_index + 1;
        }

        template<class V>
        void operator&(std::map<std::string,V>& value) {
            JArray* cur_array = _array;
            int cur_index = _index;
            JObject * item = (JObject*)(_array->get(_index));

            std::vector<std::string> keys = item->getKeys();
            for(int i = 0; i < keys.size(); i ++ ) {
                V tmp;
                value[keys[i]] = tmp;
            }

            for(typename std::map<std::string, V>::iterator iter = value.begin();
                    iter != value.end(); iter ++) {
                *this & iter->second;
            }
            _array = cur_array;
            _index = cur_index + 1;
        }


        void operator&(int& value) {
            value = ((JInt*)_array->get(_index))->getValue();
            _index ++;
        }

        void operator&(long& value) {
            value = ((JInt*)_array->get(_index))->getValue();
            _index ++;
        }

        void operator&(std::string& value) {
            value = ((JString*)_array->get(_index))->getValue();
            _index ++;
        }

        void operator&(bool& value) {
            if (_array->get(_index)->type() == VT_TRUE)
                value = true;
            else
                value = false;
            _index ++;
        }

        void operator&(double& value) {
            value = ((JInt*)_array->get(_index))->getValue();
            _index ++;
        }
    private:
        JArray* _instance;
        JArray* _array;
        int _index;
};
}
#endif
