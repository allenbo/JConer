#ifndef __JCONER_LOGGING_HPP__
#define __JCONER_LOGGING_HPP__

#include <stdarg.h>

namespace JCONER {


#define LOG_DEBUG(...) _logger.debug( __FILE__, __LINE__, __VAR_ARGS__)
#define LOG_INFO(...) _logger.info( __FILE__, __LINE__, __VAR_ARGS__)
#define LOG_WARN(...) _logger.warn( __FILE__, __LINE__, __VAR_ARGS__)
#define LOG_ERROR(...) _logger.error( __FILE__, __LINE__, __VAR_ARGS__)
#define LOG_FATAL(...) _logger.fatal( __FILE__, __LINE__, __VAR_ARGS__)

#define CLASS_MAKE_LOGGER \
    private:\
            Logger _logger;


enum Level {
    DEBUG = 0,
    INFO,
    WARN,
    ERROR,
    FATAL
};

class Logger {
    public:
        Logger()
            : _level(Level.INFO) {}

        Logger(Level level)
            : _level(level) {}

        void setLevel(Level level) { _level = level; }

        void debug(const char* fmt, ...);
        void info(const char* fmt, ...);
        void warn(const char* fmt, ...);
        void error(const char* fmt, ...);
        void fatal(const char* fmt, ...);
        
        void debug(const char* filename, int lineno, const char* fmt, ...);
        void info(const char* filename, int lineno, const char* fmt, ...);
        void warn(const char* filename, int lineno, const char* fmt, ...);
        void error(const char* filename, int lineno, const char* fmt, ...);
        void fatal(const char* filename, int lineno, const char* fmt, ...);

    private:
        Level _level;
        void _log(Level level, const char* fmt, va_list va);
        void _log(const char* filename, int lineno, Level level, const char* fmt, va_list va);
};

}

#endif
