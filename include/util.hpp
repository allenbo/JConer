#ifndef __JCONER_UTIL_HPP__
#define __JCONER_UTIL_HPP__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

namespace JCONER {

#define BUFSIZE 1024
#define MALLOC(type, ptr, size) do {\
    if (((ptr) = (type*)malloc(size)) == NULL) {\
        fprintf(stderr, "Run out of memory at %s, %d\n", __FILE__, __LINE__);\
        exit(-1);\
    }\
} while(0)

#define REALLOC(type, ptr, size)  do {\
    if (((ptr) = (type*)realloc(ptr, size)) == NULL) {\
        fprintf(stderr, "Run out of memory at %s, %d\n", __FILE__, __LINE__);\
        exit(-1);\
    }\
} while(0)

class VarString {
    public:
        VarString() {
            MALLOC(char, _p, BUFSIZE);
            _cur = _p;
            _capacity = BUFSIZE;
        }
        
        inline void append(char c) {
            if (_cur - _p == _capacity - 1) {
                REALLOC(char, _p, _capacity * 2);
                _cur = _p + _capacity - 1;
                _capacity *= 2;
            }

            *_cur++ = c;
        }

        void append(const char* p) {
            int len = strlen(p);
            append(p, len);
        }

        void append(const char* p, int n) {
            if (_cur - _p + n > _capacity - 1) {
                int cur_size = _cur - _p;
                int new_length = 0;
                if (_cur - _p + n < _capacity * 2 - 1) {
                    new_length = _capacity * 2; 
                } else {
                    new_length = cur_size + n + BUFSIZE;
                }

                REALLOC(char, _p, new_length);
                _cur = _p + cur_size;
                _capacity = new_length;
            }
            memcpy(_cur, p, n); 
            _cur += n;
        }

        inline std::string toString() {
            std::string s(_p, _cur - _p);
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
        VarString(const VarString&);
        VarString operator=(const VarString&);
};

class HexCode {
    public:
        static int32_t decode(const char* str) {
            int32_t value = 0;
            for(int i = 0; i < 4; i ++ ) {
                value <<= 4;
                char c = str[i];
                if (isdigit(c)){
                    value += c - '0';
                } else if ('A' <= c && c <= 'F') { 
                    value += c - 'A' + 10;
                } else if ('a' <=c && c <= 'f') {
                    value += c - 'a' + 10;
                } else {
                    return -1;
                }
            }
            return value;
        }
};

class UTF8 {
    public:
        static int encode(int32_t value, char* str, int n) {
            if (value < 0 || n <= 0)
                return -1;
            if (value < 0x80) {
                str[0] = (char)value;
                return 1;
            }
            if (value < 0x800) {
                if (n < 2) return -1;
                str[0] = 0xC0 + ((value & 0x7C0) >> 6);
                str[1] = 0x80 + ((value & 0x03F));
                return 2;
            }
            if (value < 0x10000) {
                if (n < 3) return -1;
                str[0] = 0xE0 + ((value & 0XF000) >> 12);
                str[1] = 0x80 + ((value & 0x0FC0) >> 6);
                str[2] = 0x80 + ((value & 0x003F));
                return 3;
            }
            if (value <= 0x10FFFF) {
                if (n < 4) return -1;
                str[0] = 0xF0 + ((value & 0x1C0000) >> 18);
                str[1] = 0x80 + ((value & 0x03F000) >> 12);
                str[2] = 0x80 + ((value & 0x000FC0) >> 6);
                str[3] = 0x80 + ((value & 0x00003F));
                return 4;
            }
            return -1;
        }
    
        static int decode(const char* p, int32_t* hex) {
            int count = getUTF8Length(p);
            if (count  == 0) {
                return 0;
            }
            if (count == 1) {
                *hex = (int32_t)*p;
            } else {
                int32_t value = 0;
                unsigned char u = (unsigned char)*p;

                if (count == 2) value = u & 0x1F;
                else if (count == 3) value = u & 0xF;
                else value = u & 0x7;

                for (int i = 1; i < count; i ++ ) {
                    u = (unsigned char)p[i];
                    if (u < 0x80 || u > 0xBF) return 0;
                    value = (value << 6) + (u & 0x3F);
                }

                if (value > 0x10FFFF) return 0;
                if (0xD800 <= value && value <= 0xDFFF) return 0;        
                if ((count == 2 && value < 0x80) ||
                    (count == 3 && value < 0x800) ||
                    (count == 4 && value < 0x10000)) return 0;
                *hex = value;
            }
            return count;
        }
    private:
        static int getUTF8Length(const char* p) {
            unsigned char byte = (unsigned char)*p;
            if (byte < 0x80) return 1;
            if (0x80 <= byte && byte <= 0xBF) return 0;
            if (byte == 0xC0 || byte == 0xC1) return 0;
            if (0xC2 <= byte && byte <= 0xDF) return 2;
            if (0xE0 <= byte && byte <= 0xEF) return 3;
            if (0xF0 <= byte && byte <= 0xF4) return 4;
            return 0;
        }
};

}
#endif
