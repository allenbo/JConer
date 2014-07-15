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

void JNull::printout() {
    printf("null");
}
// JInt definition
JInt::JInt(const long value)
    :JValue(VT_INTEGER), _value(value)
{
}

void JInt::printout() {
    printf("int[%ld]", _value);
}
// JReal definition
JReal::JReal(const double value)
    :JValue(VT_REAL), _value(value)
{
}

void JReal::printout() {
    printf("real[%f]", _value);
}

// JString definition
JString::JString(const std::string value)
    :JValue(VT_STRING), _value(value)
{
}

JString::JString(const char* str)
    :JValue(VT_STRING), _value(str)
{
}

void JString::printout() {
    printf("string[%s]", _value.c_str());
}

// JTrue definition
JTrue::JTrue()
    :JValue(VT_TRUE)
{
}

void JTrue::printout() {
    printf("true");
}

// JFalse definition
JFalse::JFalse()
    :JValue(VT_FALSE)
{
}

void JFalse::printout() {
    printf("false");
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

InsertError JArray::append(JValue* element) {
    if (element == this) return IE_SELF;
    if (element == NULL) return IE_NULL;
    _array.push_back(element);
    return IE_SUC;
}

void JArray::append(const long value) {
    JValue* elt = new JInt(value);
    _array.push_back(elt);
}

void JArray::append(const int value) {
    JValue* elt = new JInt(value);
    _array.push_back(elt);
}

void JArray::append(const std::string value) {
    JValue* elt = new JString(value);
    _array.push_back(elt);
}

void JArray::append(const char* str) {
    _array.push_back(new JString(str));
}

void JArray::append(const double value) {
    JValue* elt = new JReal(value);
    _array.push_back(elt);
}
void JArray::append(const bool value) {
    JValue* elt = NULL;
    if (value) {
        elt = new JTrue();
    } else {
        elt = new JFalse();
    }
    _array.push_back(elt);
}

void JArray::appendNull() {
    JValue* elt = new JNull();
    _array.push_back(elt);
}

void JArray::printout() {
    printf("[");
    for(size_t i = 0; i < _array.size(); i ++) {
        _array[i]->printout();
        if (i != _array.size() - 1)
            printf(",");
    }
    printf("]");
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

InsertError JObject::put(const std::string key, JValue* value) {
    if (value == this) return IE_SELF;
    if (value == NULL) return IE_NULL;
    _object[key] = value;
    return IE_SUC;
}

void JObject::put(const std::string key, const long value) {
    _object[key] = new JInt(value);
}

void JObject::put(const std::string key, const int value) {
    _object[key] = new JInt(value);
}

void JObject::put(const std::string key, const std::string value) {
    _object[key] = new JString(value);
}

void JObject::put(const std::string key, const char* str) {
    _object[key] = new JString(str);
}

void JObject::put(const std::string key, const double value) {
    _object[key] = new JReal(value);
}

void JObject::put(const std::string key, const bool value)  {
    if (value) {
        _object[key] = new JTrue();
    } else {
        _object[key] = new JFalse();
    }
}

void JObject::put(const std::string key) {
    _object[key] = new JNull();
}

void JObject::printout() {
    printf("{");
    for(std::map<std::string, JValue*>::iterator iter = _object.begin();
            iter != _object.end(); iter ++) {
        printf("%s:", iter->first.c_str());
        iter->second->printout();
        printf(",");
    }
    printf("}");
}

JObject::~JObject() {
    for(std::map<std::string, JValue*>::iterator iter = _object.begin();
            iter != _object.end(); iter ++) {
        delete iter->second;
    }
}

}
