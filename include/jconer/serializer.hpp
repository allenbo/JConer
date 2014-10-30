#ifndef __JCONER_OBJECT_HPP__
#define __JCONER_OBJECT_HPP__

#include "jconer/value.hpp"
#include <map>
#include <set>
#include <list>
#include <vector>
#include <cassert>

namespace JCONER {

class OutSerializer {
    public:
        OutSerializer() {
            clear(); 
        }

        JValue* get_content() { return _array; }

        void clear() {
          _array = new JArray();
          _curr = _array;
        }

        template<class Type>
        void operator&(Type& value) {
            JArray* item = new JArray();
            JArray* old_curr = _curr;
            _curr->append(item);
            _curr = item;

            value.serialize(*this);
            _curr = old_curr;
        }

        template<template<class T, class Allocator> class Container, class ValueType>
        void operator&(Container<ValueType, std::allocator<ValueType> >& value) {
            JArray* item = new JArray();
            JArray* old_curr = _curr;
            _curr->append(item);
            _curr = item;

            for(typename Container<ValueType, std::allocator<ValueType> >::iterator iter = value.begin();
                    iter != value.end(); iter ++) {
                *this & *iter;
            }
            _curr = old_curr;
        }

        template<class K, class V>
        void operator&(std::pair<K, V>& value) {
            JArray* item = new JArray();
            JArray* old_curr = _curr;
            _curr->append(item);
            _curr = item;

            *this & value.first;
            *this & value.second;
            _curr = old_curr;
        }

        template<class K, class V>
        void operator&(std::map<K,V>& value) {
            JArray* item = new JArray();
            JArray* old_curr = _curr;
            _curr->append(item);
            _curr = item;

            for(typename std::map<K, V>::iterator iter = value.begin();
                    iter != value.end(); iter ++) {
                *this & *iter;
            }
            _curr = old_curr;
        }

        void operator&(const int value) {
            _curr->append(value);
        }

        void operator&(const long value) {
            _curr->append(value);
        }

        void operator&(const std::string value) {
            _curr->append(value);
        }

        void operator&(const bool value) {
            _curr->append(value);
        }

        void operator&(const char* value) {
            _curr->append(value);
        }

        void operator&(const double value) {
            _curr->append(value);
        }

        void operator&(const float value) {
            _curr->append(value);
        }


    private:
        JArray* _array;
        JArray* _curr;
};


class InSerializer {
    public:
        InSerializer(JValue* array) {
            assert(array->isArray());
            _array = (JArray*)array;
            _curr = _array;
            _index = 0;
        }

        template<class Type>
        void operator&(Type& value) {
            JArray* old_curr = _curr;
            _curr = (JArray*)_curr->get(_index);

            int old_index = _index;
            _index = 0;
            
            value.serialize(*this);

            _curr = old_curr;
            _index = old_index + 1;
        }

        template<template<class T, class Allocator> class Container, class ValueType>
        void operator&(Container<ValueType, std::allocator<ValueType> >& value) {
            value.clear();
            JArray* old_curr  = _curr;
            _curr = (JArray*)_curr->get(_index);

            int old_index = _index;
            _index = 0;

            for(int i = 0; i < _curr->size(); i ++) {
                ValueType tmp;
                value.push_back(tmp);
            }

            for(typename Container<ValueType, std::allocator<ValueType> >::iterator iter = value.begin();
                    iter != value.end(); iter ++) {
                *this & *iter;
            }
            _curr = old_curr;
            _index = old_index + 1;
        }

        template<class K, class V>
        void operator&(std::pair<K, V>& value) {
            JArray* old_curr  = _curr;
            _curr = (JArray*)_curr->get(_index);

            int old_index = _index;
            _index = 0;

            *this & value.first;
            *this & value.second;

            _curr = old_curr;
            _index = old_index + 1;

        }

        template<class K, class V>
        void operator&(std::map<K,V>& value) {
            value.clear();
            JArray* old_curr  = _curr;
            _curr = (JArray*)_curr->get(_index);

            int old_index = _index;
            _index = 0;

            for(int i = 0; i < _curr->size(); i ++) {
                K k;
                V v;
                std::pair<K, V> p = std::make_pair(k, v);
                *this & p;
                value.insert(p);
            }

            _curr = old_curr;
            _index = old_index + 1;
        }


        void operator&(int& value) {
            value = _curr->get(_index)->getInteger();
            _index ++;
        }

        void operator&(long& value) {
            value = _curr->get(_index)->getInteger();
            _index ++;
        }

        void operator&(std::string& value) {
            value = _curr->get(_index)->getString();
            _index ++;
        }

        void operator&(bool& value) {
            if (_array->get(_index)->isTrue())
                value = true;
            else
                value = false;
            _index ++;
        }

        void operator&(double& value) {
            value = _curr->get(_index)->getReal();
            _index ++;
        }

        void operator&(float& value) {
            value = _curr->get(_index)->getReal();
            _index ++;
        }
    private:
        JArray* _array;
        JArray* _curr;
        int _index;
};
}
#endif
