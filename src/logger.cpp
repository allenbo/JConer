#include <logging.hpp>
#include <stdlib.h>
#include <stdio.h>

namespace JCONER {

void Logger::debug(const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    _log(DEBUG, fmt, va);
    va_end(va);
}

void Logger::info(const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    _log(INFO, fmt, va);
    va_end(va);
}

void Logger::warn(const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    _log(WARN, fmt, va);
    va_end(va);
}

void Logger::error(const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    _log(ERROR, fmt, va);
    va_end(va);
}

void Logger::fatal(const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    _log(FATAL, fmt, va);
    va_end(va);
}

void Logger::debug(const char* filename, int lineno, const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    _log(filename, lineno, DEBUG, fmt, va);
    va_end(va);
}

void Logger::info(const char* filename, int lineno, const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    _log(filename, lineno, INFO, fmt, va);
    va_end(va);
}

void Logger::warn(const char* filename, int lineno, const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    _log(filename, lineno, WARN, fmt, va);
    va_end(va);
}

void Logger::error(const char* filename, int lineno, const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    _log(filename, lineno, ERROR, fmt, va);
    va_end(va);
}

void Logger::fatal(const char* filename, int lineno, const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    _log(filename, lineno, FATAL, fmt, va);
    va_end(va);
}

void Logger::_log(Level level, const char* fmt, va_list va) {
    if (level < _level) return;
    fprintf(stderr, fmt, va);
    
    if (level == ERROR or level == FATAL) {
        exit(-1);
    }
}

void Logger::_log(const char* filename, int line, Level level, const char* fmt, va_list va) {
    if (level < _level) return;
    char fullfmt[512];
    snprintf(fullfmt, 511, "[%s|%d]%s", filename, line, fmt); 
    fprintf(stderr, fullfmt, va);
    
    if (level == ERROR or level == FATAL) {
        exit(-1);
    }
}

}
