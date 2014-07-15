JConer
======

A C++ implementation for Json

Examples
------
**Load from file**
```
#include "json.hpp"
using namespace JCONER;
JValue* rst = load("myjsonfile");
delete rst;
```

**dump to file**
```
#include "json.hpp"
using namespace JCONER;
JValue* item = any_function_or_operation_that_returns_a_json_value();
dump(item, std::cout);
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
