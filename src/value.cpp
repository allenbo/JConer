#include "value.hpp"

namespace JCONER {

// JValue definition
JValue::JValue(ValueType type)
    :_type(type)
{
}

JValue::~JValue() {
}

// JNull definition
JNull::JNull()
    :JValue(VT_NULL)
{
}

// JInt definition
JInt::JInt(long value)
    :JValue(VT_INTEGER), _value(value)
{
}

// JReal definition
JReal::JReal(double value)
    :JValue(VT_REAL), _value(value)
{
}

// JString definition
JString::JString(std::string value)
    :JValue(VT_STRING), _value(value)
{
}

// JTrue definition
JTrue::JTrue()
    :JValue(VT_TRUE)
{
}

// JFalse definition
JFalse::JFalse()
    :JValue(VT_FALSE)
{
}

// JArray definition
JArray::JArray()
    :JValue(VT_ARRAY)
{
}

JArray::JArray(const std::vector<JValue*>& array)
    :JValue(VT_ARRAY), _array(array)
{
}

void JArray::append(JValue* element) {
    _array.push_back(element);
}

JArray::~JArray() {
    for(size_t i = 0; i < _array.size(); i ++ ) {
        delete _array[i];
    }
}

// JObject definition
JObject::JObject()
    :JValue(VT_OBJECT)
{
}

JObject::JObject(const std::map<std::string, JValue*>& object)
    :JValue(VT_OBJECT)
{
    _object.insert(object.begin(), object.end());
}

void JObject::put(std::string key, JValue* value) {
    _object[key] = value;
}

JObject::~JObject() {
    for(std::map<std::string, JValue*>::iterator iter = _object.begin();
            iter != _object.end(); iter ++) {
        delete iter->second;
    }
}

}
