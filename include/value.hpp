#ifndef __JCONER_VALUE_HPP__
#define __JCONER_VALUE_HPP__

#include <string>
#include <map>
#include <vector>

#include "logging.hpp"
#define NULL_VALUE 0

namespace JCONER {

typedef int NULL_TYPE;

enum ValueType {
    VT_OBJECT = 0,
    VT_ARRAY,
    VT_INTEGER,
    VT_STRING,
    VT_REAL,
    VT_TRUE,
    VT_FALSE,
    VT_NULL
};

enum InsertError {
    IE_SUC = 0,
    IE_SELF,
    IE_NULL
};

class JValue {
    public:
        JValue(ValueType type);
        inline ValueType type() { return _type; }
        virtual ~JValue();
        virtual void printout() = 0;

        static inline bool isString(const JValue* value) { return value->_type == VT_STRING;}
        static inline bool isInteger(const JValue* value) { return value->_type == VT_INTEGER;}
        static inline bool isReal(const JValue* value) { return value->_type == VT_REAL;}
        static inline bool isTrue(const JValue* value) { return value->_type == VT_TRUE;}
        static inline bool isFalse(const JValue* value) { return value->_type == VT_FALSE;}
        static inline bool isNull(const JValue* value) { return value->_type == VT_NULL;}
        static inline bool isObject(const JValue* value) { return value->_type == VT_OBJECT;}
        static inline bool isArray(const JValue* value) { return value->_type == VT_ARRAY;}

    protected:
        ValueType _type;
};

class JNull : public  JValue {
    public:
        JNull();
        inline NULL_TYPE getValue() { return NULL_VALUE; }
        void printout();
};

class JInt : public JValue {
    public:
        JInt(const long value);
        inline long getValue() { return _value; }
        void printout();
    private:
        long _value;
};

class JReal : public JValue {
    public:
        JReal(const double value);
        inline double getValue() { return _value; }
        void printout();
    private:
        double _value;
};

class JString : public JValue {
    public:
        JString(const std::string value);
        JString(const char* str);
        inline std::string getValue() { return _value; }
        std::string getAsciiValue();
        void printout();
    private:
        std::string _value;
};

class JTrue : public JValue {
    public:
        JTrue();
        inline bool getValue() { return true; }
        void printout();
    private:
};

class JFalse : public JValue {
    public:
        JFalse();
        inline bool getValue() { return true; }
        void printout();
    private:
        bool _value;
};

class JArray : public JValue {
    CLASS_MAKE_LOGGER
    public:
        JArray();
        JArray(const std::vector<JValue*>& array);
        ~JArray();

        inline std::vector<JValue*> getArray() { return _array; }
        inline int size() { return _array.size(); }
        void printout();
        inline JValue* get(int i) {
            if (i < _array.size())
                return _array[i];
            else
                return NULL;
        }

        InsertError append(JValue* element);
        void append(const long value);
        void append(const int value);
        void append(const std::string value);
        void append(const char* str);
        void append(const bool value);
        void append(const double value);
        void appendNull();
    private:
        std::vector<JValue*> _array;
};

class JObject : public JValue {
    public:
        JObject();
        JObject(const std::map<std::string, JValue*>& object);
        ~JObject();

        inline std::map<std::string, JValue*> getObject() { return _object; }
        inline int size() { return _object.size(); }
        void printout();
        inline JValue* get(std::string key) {
            if (_object.find(key) != _object.end()) {
                return _object[key];
            } else {
                return NULL;
            }
        }

        std::vector<std::string> getKeys() {
            std::vector<std::string> keys;
            for(std::map<std::string, JValue*>::iterator iter = _object.begin();
                    iter != _object.end(); iter ++ ) {
                keys.push_back(iter->first);
            }
            return keys;
        }

        InsertError put(const std::string, JValue*); 
        void put(const std::string, const long);
        void put(const std::string, const int);
        void put(const std::string, const std::string);
        void put(const std::string, const char*);
        void put(const std::string, const bool);
        void put(const std::string, const double);
        void put(const std::string);
    private:
        std::map<std::string, JValue*> _object;
};

}

#endif
