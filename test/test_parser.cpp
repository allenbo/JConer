#include "jconer/json.hpp"
using namespace JCONER;

int main(int argc, char** argv) {
    //FileIStream fin(argv[1]);
    //Parser parser(fin);
    Parser parser;

    JValue *rst = parser.parseFromFile(argv[1]);
    if (rst->type() == VT_ARRAY) {
        JArray* arr_value = (JArray*) rst;
        arr_value->append(128);
        arr_value->append(12.8);
        arr_value->append("allenbo");
        JObject* obj_value = new JObject();
        obj_value->put("name", "allenbo");
        obj_value->put("age", 23);
        arr_value->append(obj_value);
    }
    rst->printout();
    printf("\n");
}
