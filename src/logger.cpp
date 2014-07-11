#include <logging.hpp>
#include <stdlib.h>

namespace JCONER {

void Logger::debug(const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    _log(Level.DEBUG, fmt, va);
    va_end(va);
}

void Logger::info(const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    _log(Level.INFO, fmt, va);
    va_end(va);
}

void Logger::warn(const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    _log(Level.WARN, fmt, va);
    va_end(va);
}

void Logger::error(const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    _log(Level.ERROR, fmt, va);
    va_end(va);
}

void Logger::fatal(const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    _log(Level.FATAL, fmt, va);
    va_end(va);
}

void Logger::debug(const char* filename, int lineno, const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    _log(filename, lineno, Level.DEBUG, fmt, va);
    va_end(va);
}

void Logger::info(const char* filename, int lineno, const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    _log(filename, lineno, Level.INFO, fmt, va);
    va_end(va);
}

void Logger::warn(const char* filename, int lineno, const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    _log(filename, lineno, Level.WARN, fmt, va);
    va_end(va);
}

void Logger::error(const char* filename, int lineno, const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    _log(filename, lineno, Level.ERROR, fmt, va);
    va_end(va);
}

void Logger::fatal(const char* filename, int lineno, const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    _log(filename, lineno, Level.FATAL, fmt, va);
    va_end(va);
}

void Logger::_log(Level level, const char* fmt, va_list va) {
    if (level < _level) return;
    fprintf(stderr, fmt, va);
    
    if (level == Level.ERROR or level == Level.FATAL) {
        exit(-1);
    }
}

void Logger::_log(const char* filename, int line, Level level, const char* fmt, va_list va) {
    if (level < _level) return;
    char fullfmt[512];
    snprintf(fullfmt, 511, "[%s|%d]%s", filename, line, fmt); 
    fprintf(stderr, fullfmt, va);
    
    if (level == Level.ERROR or level == Level.FATAL) {
        exit(-1);
    }
}

}
