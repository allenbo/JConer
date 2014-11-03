#include "jconer/value.hpp"
#include "jconer/util.hpp"
#include "common/all.hpp"

using namespace COMMON;
namespace JCONER {

// JValue definition
JValue::JValue(ValueType type)
    :_type(type)
{
}

std::string JValue::getString() {
    return ((JString*)this)->getValue();
}

long JValue::getInteger() {
    return ((JInt*)this)->getValue();
}

bool JValue::getBool() {
    return _type == VT_TRUE ? true : false;
}

double JValue::getReal() {
    return ((JReal*)this)->getValue();
}

JValue* JValue::get(size_t i) {
  return ((JArray*)this)->get(i);
}

JValue* JValue::get(std::string key) {
  return ((JObject*)this)->get(key);
}

std::vector<std::string> JValue::getKeys() {
  return ((JObject*)this)->getKeys();
}

bool JValue::contain(std::string key) {
  return ((JObject*)this)->contain(key);
}

int JValue::size() {
  if (isObject()) {
    return ((JObject*)this)->size();
  } else {
    return ((JArray*)this)->size();
  }
}

JValue::~JValue() {
}

// JNull definition
JNull::JNull()
    :JValue(VT_NULL)
{
}

JNull JNull::_instance;

void JNull::printout() {
    printf("null");
}

JValue* JNull::deepcopy() {
    return JNull::getInstance();
}

// JInt definition
JInt::JInt(const long value)
    :JValue(VT_INTEGER), _value(value)
{
}

void JInt::printout() {
    printf("int[%ld]", _value);
}

JValue* JInt::deepcopy() {
    return new JInt(_value);
}

// JReal definition
JReal::JReal(const double value)
    :JValue(VT_REAL), _value(value)
{
}

void JReal::printout() {
    printf("real[%f]", _value);
}

JValue* JReal::deepcopy() {
    return new JReal(_value);
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

std::string JString::getAsciiValue() {
    VarString vs;
    int i = 0;
    char c;
    const char* p = _value.c_str();
    int len = _value.size();
    while (i < len) {
        c = p[i];
        switch(c){
            case '"': vs.append("\\\""); i ++; break;
            case '/': vs.append("\\/"); i++; break;
            case '\\': vs.append("\\\\"); i++; break;
            case '\t': vs.append("\\t"); i++; break;
            case '\b': vs.append("\\b"); i++; break;
            case '\f': vs.append("\\f"); i++; break;
            case '\n': vs.append("\\n"); i++; break;
            case '\r': vs.append("\\r"); i++; break;
            default:{
                if ((unsigned)c <= 0x80) {
                    vs.append(c);
                    i ++;
                } else {
                    char buffer[13];
                    int32_t value = 0;
                    int count = UTF8::decode(p + i, &value);
                    i += count;
                    if (value < 0x10000){
                       sprintf(buffer, "\\u%04X", value); 
                       vs.append(buffer, 6);
                    } else {
                        int32_t first, second;
                        value -= 0x10000;
                        first = 0xD800 | ((value & 0xFFC00) >> 10);
                        second = 0xDC00 | (value & 0x003FF);

                        sprintf(buffer, "\\u%04X\\u%04X", first, second);
                        vs.append(buffer, 12);
                    }
                }
            }
        }
    }
    return vs.toString();
}

JValue* JString::deepcopy() {
    return new JString(_value);
}

// JTrue definition
JTrue::JTrue()
    :JValue(VT_TRUE)
{
}

JTrue JTrue::_instance;

void JTrue::printout() {
    printf("true");
}

JValue* JTrue::deepcopy()  {
    return JTrue::getInstance();
}

// JFalse definition
JFalse::JFalse()
    :JValue(VT_FALSE)
{
}

JFalse JFalse::_instance;

void JFalse::printout() {
    printf("false");
}

JValue* JFalse::deepcopy()  {
    return JFalse::getInstance();
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

void JArray::append(const size_t value) {
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
        elt = JTrue::getInstance();
    } else {
        elt = JFalse::getInstance();
    }
    _array.push_back(elt);
}

void JArray::appendNull() {
    JValue* elt = JNull::getInstance();
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
        if (_array[i]->isFalse() || _array[i]->isTrue() || _array[i]->isNull()) continue;
        delete _array[i];
    }
}

JValue* JArray::pop(size_t i) {
    if (i < _array.size()) {
        JValue* rst = _array[i];
        _array.erase(_array.begin() + i);
        return rst;
    }
    return NULL;
}
JValue* JArray::pop_back() {
    JValue* rst = _array.back();
    _array.pop_back();
    return rst;
}

JValue* JArray::deepcopy() {
    JArray* rst = new JArray();
    for(size_t i = 0; i < _array.size(); i ++ ) {
        rst->append(_array[i]->deepcopy());
    }
    return rst;
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

std::vector<std::string> JObject::getKeys() {
    std::vector<std::string> keys;
    for(std::map<std::string, JValue*>::iterator iter = _object.begin();
            iter != _object.end(); iter ++ ) {
        keys.push_back(iter->first);
    }
    return keys;
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

void JObject::put(const std::string key, const size_t value) {
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
        _object[key] = JTrue::getInstance();
    } else {
        _object[key] = JFalse::getInstance();
    }
}

void JObject::put(const std::string key) {
    _object[key] = JNull::getInstance();
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
        if (iter->second->isFalse() || iter->second->isTrue() || iter->second->isNull()) continue;
        delete iter->second;
    }
}

JValue* JObject::pop(std::string key) {
    if (contain(key)) {
        JValue* rst = _object[key];
        _object.erase(key);
        return rst;
    }
    return NULL;
}

JValue* JObject::deepcopy() {
    JObject* rst = new JObject();
    for(std::map<std::string, JValue*>::iterator iter = _object.begin();
            iter != _object.end(); iter ++) {
        rst->put(iter->first, iter->second->deepcopy());
    }
    return rst;
}

}
