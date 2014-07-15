#include "parser.hpp"
#include "stream.hpp"
#include "value.hpp"
#include "dump.hpp"
#include <iostream>

using namespace JCONER;

int main(int argc, char** argv) {
    JArray* arr_value = new JArray();
    arr_value->append(128);
    arr_value->append(12.8);
    arr_value->append("allenbo");

    JObject* obj_value = new JObject();
    obj_value->put("name", "allenbo");
    obj_value->put("age", 23);

    arr_value->append(obj_value);
    
    //arr_value->printout();
    dumpFile(arr_value, "myjson", DUMP_PRETTY_PRINT);
    char* str = NULL;
    dumps(arr_value, &str);
    std::cout << str << std::endl;
    free(str);
    delete arr_value;
}
