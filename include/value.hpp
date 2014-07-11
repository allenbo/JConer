#ifndef __JCONER_JSON_HPP__
#define __JCONER_JSON_HPP__


namespace JCONER {

typedef int NULL_TYPE;
#define NULL_VALUE 0

enum ValueType {
    VT_OBJECT = 0,
    VT_ARRAY,
    VT_INTEGER,
    VT_STRING,
    VT_REAL,
    VT_BOOLEAN,
    VT_NULL;
};

class JValue {
    public:
        Value(ValueType type);
        inline ValueType getType() { return _type; }
        virtual ~Value();
    protected:
        ValueType _type;
};

class JNull : public  JValue {
    public:
        JNull();
        inline NULL_TYPE getType() { return NULL_VALUE; }
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

class JBoolean : public JValue {
    public:
        JBoolean(bool value);
        inline bool getValue() { return _value; }
    private:
        bool _value;
};

class JArray : public JValue {
    public:
        JArray();
        JArray(const std::vector<JValue*>& array);
        inline std::vector<JValue*> getArray() { return _array; }
        ~JArray();
    private:
        std::vetor<JValue*> _array;
};

class JObject : public JValue {
    public:
        JObject();
        JObject(const std::map<std::string, JValue*>& object);
        inline std::map<std::string,, JValue*> getObject() { return _object; }
        ~JObject();
    private:
        std::map<std::string, JValue*> _object;
};

}

#endif
