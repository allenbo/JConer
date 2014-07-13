#ifndef __JCONER_UTIL_HPP__
#define __JCONER_UTIL_HPP__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

namespace JCONER {

#define BUFSIZE 1024
#define MALLOC(ptr, size) do {\
    if (((ptr) = malloc(size)) == NULL) {\
        fprintf(stderr, "Run out of memory at %s, %d\n", __FILE__, __LINE__);\
        exit(-1);\
    } while(0)

#define REALLOC(ptr, size)  do {\
    if (((ptr) = realloc(size)) == NULL) {\
        fprintf(stderr, "Run out of memory at %s, %d\n", __FILE__, __LINE__);\
        exit(-1);\
    } while(0)

class VarString {
    public:
        VarString() {
            MALLOC(_p, BUFSIZE);
            _cur = _p;
            _capacity = BUFSIZE;
        }
        
        inline void append(char c) {
            if (_cur - _p == _capacity - 1) {
                REALLOC(_p, _capacity * 2);
                _cur = _p + _capacity - 1;
                _capacity *= 2;
            }

            *_cur = c;
        }

        inline void append(char* p, int n) {
            if (_cur - _p + n > _capacity - 1) {
                int cur_size = _cur - _p;
                int new_length = 0;
                if (_cur - _p + n < _capacity * 2 - 1) {
                    new_length = _capacity * 2; 
                } else {
                    new_lenght = cur_size + n + BUFSIZE;
                }

                REALLOC(_p, new_length);
                _cur = _p + cur_size;
                _capacity = new_length;
            }
            memcpy(_cur, p, n); 
            _cur += n;
        }

        inline std::string toString() {
            string s(_p, _cur - _p);
            return s;
        }

        inline int size() const { return _cur - _p; }

        ~VarString() {
            free(_p);
        }
    private:
        char* _p;
        char* _cur;
        int _capacity;
        VarString(const VarString&) = 0;
        VarString operator=(const VarString&) = 0;
};

}
#endif
