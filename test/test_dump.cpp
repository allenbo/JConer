#include <iostream>
#include "jconer/json.hpp"

using namespace JCONER;

int main(int argc, char** argv) {
    JArray* arr_value = new JArray();
    arr_value->append(128);
    arr_value->append(12.8);
    arr_value->append("Allenbo\tLin");

    JObject* obj_value = new JObject();
    obj_value->put("name", "杩");
    obj_value->put("age", 23);

    arr_value->append(obj_value);
    
    arr_value->printout();
    dumpFile(arr_value, "myjson", DUMP_COMPACT_PRINT);
    char* str = NULL;
    dumps(arr_value, &str);
    std::cout << str << std::endl;
    free(str);
    delete arr_value;
    //JValue* value = load(argv[1]);
    //char* str = NULL;
    //dumpFile(value, "myjson");
    //dumps(value, &str);
    //std::cout << str << std::endl;
    //free(str);
    //delete value;
}

