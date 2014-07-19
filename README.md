JConer
======

A C++ implementation for Json

Examples
------
**Load from file and buffer**
```
#include "json.hpp"
using namespace JCONER;
PError err;
JValue* rst = load("myjsonfile", err);
if (NULL == rst) {
  std::cout << err.text;
} else {
  delete rst;
}
err.clear();
rst = loads("[128, 256]", err);
if (NULL != rst) delete rst;
```

**dump to file and buffer**
```
#include "json.hpp"
using namespace JCONER;
JValue* item = any_function_or_operation_that_returns_a_json_value();
dump(item, std::cout);
char* buffer;
dumps(item, &buffer);
std::cout << buffer << std::endl;
free(buffer);
delete item;
```
JCONER VALUES
------
```
JValue: the base class of all other JCONER value
JInt: type() == VT_INTEGER
JString: type() == VT_STRING
JReal: type() == VT_REAL
JTrue: type() == VT_TRUE
JFalse: type() == VT_FALSE
JNull: type() == VT_NULL
JArrty: type() == VT_ARRAY
JObject: type() == VT_OBJECT
```
Manipulation of array and object
------
**JArray**
```
JArray * array_item = new JArray();
array_item.append(int_value);
array_item.append(string_value);
array_item.append(bool_value);
array_item.append(); // append JNull
array_item.append(real_value);
array_item.append(other_jvalue_ptr);
delete array_item;
```
**JObject**
```
JObject* obj_item = new JObject();
obj_item.put("an_integer", int_value);
obj_item.put("an_string", string_value);
obj_item.put("an_null");
obj_item.put("an_jvalue", other_jvalue_ptr);
```
Serializer
------
JConer provides a convenient serializer to convert user-defined object to JArray. Here is how it works.
```
#include "json.hpp"

class Helper {
    public:
        Helper() {
            _x = _y = 0;
        }

        template<class Serializer>
        JArray* serialize(Serializer& serializer) {
            serializer & _x;
            serializer & _y;
        }
    private:
        int _x, _y;
};

class Test {
    public:
        template<class Serializer>
        JArray* serialize(Serializer& serializer) {
            serializer & _int_value;
            serializer & _real_value;
            serializer & _string_value;
            serializer & _vector_value;
            serializer & _primitive_map_value;
            serializer & _user_defined_map_value;
        }
    private:
        int _int_value;
        double _real_value;
        std::string _strin_value;
        std::vector<int> _vector_value;
        std::map<std::string, int> _primitive_map_value; //key has to be string
        std::map<std::string, Helper> _user_defined_map_value;
};


int main() {
    OutSerializer sout;
    Test test;
    JArray* rst = sout & test;
    dump(rst, std::cout, DUMP_PRETTY_PRINT | DUMP_ENSURE_ASCII);
    Test another_test;
    InSerializer sin;
    another_test & sin;
    assert (another_test == test);
    delete rst;
}
```
All you have to do is to define a serialize method for every class you want to serialize.
