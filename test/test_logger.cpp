#include "logging.hpp"
using namespace JCONER;

class Test{
    public:
        Test() {
            _logger.setLevel(DEBUG);
        }
        void test_debug() {
            LOG_DEBUG("%s!\n", "filename");
            LOG_DEBUG("%d!\n", 10);
            LOG_DEBUG("%f!\n", 12.1f);
            LOG_INFO("%s\n", "This is a info message");

            _logger.setLevel(INFO);
            LOG_DEBUG("%s\n", "should not appear");
            LOG_WARN("%s\n", "There is a warn message");

            LOG_FATAL("fatal message %s\n", "death");
        }
    CLASS_MAKE_LOGGER
};

int main() {
    Test test;
    test.test_debug();
}
