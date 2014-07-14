#ifndef __JCONER_JSON_HPP__
#define __JCONER_JSON_HPP__

#include <string>
#include <map>
#include <vector>

namespace JCONER {

typedef int NULL_TYPE;
#define NULL_VALUE 0

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

class JValue {
    public:
        JValue(ValueType type);
        inline ValueType getType() { return _type; }
        virtual ~JValue();

        static inline bool isString(JValue* value) { return value->_type == VT_STRING;}
        static inline bool isInteger(JValue* value) { return value->_type == VT_INTEGER;}
        static inline bool isReal(JValue* value) { return value->_type == VT_REAL;}
        static inline bool isTrue(JValue* value) { return value->_type == VT_TRUE;}
        static inline bool isFalse(JValue* value) { return value->_type == VT_FALSE;}
        static inline bool isNull(JValue* value) { return value->_type == VT_NULL;}
        static inline bool isObject(JValue* value) { return value->_type == VT_OBJECT;}
        static inline bool isArray(JValue* value) { return value->_type == VT_ARRAY;}

    protected:
        ValueType _type;
};

class JNull : public  JValue {
    public:
        JNull();
        inline NULL_TYPE getValue() { return NULL_VALUE; }
};

class JInt : public JValue {
    public:
        JInt(long value);
        inline long getValue() { return _value; }
    private:
        long _value;
};

class JReal : public JValue {
    public:
        JReal(double value);
        inline double getValue() { return _value; }
    private:
        double _value;
};

class JString : public JValue {
    public:
        JString(std::string value);
        inline std::string getValue() { return _value; }
    private:
        std::string _value;
};

class JTrue : public JValue {
    public:
        JTrue();
        inline bool getValue() { return true; }
    private:
};

class JFalse : public JValue {
    public:
        JFalse();
        inline bool getValue() { return true; }
    private:
        bool _value;
};

class JArray : public JValue {
    public:
        JArray();
        JArray(const std::vector<JValue*>& array);
        void append(JValue* element);
        inline std::vector<JValue*> getArray() { return _array; }
        ~JArray();
    private:
        std::vector<JValue*> _array;
};

class JObject : public JValue {
    public:
        JObject();
        JObject(const std::map<std::string, JValue*>& object);
        void put(std::string, JValue*); 
        inline std::map<std::string, JValue*> getObject() { return _object; }
        ~JObject();
    private:
        std::map<std::string, JValue*> _object;
};

}

#endif
