#ifndef UTL_LOGGER_H
#define UTL_LOGGER_H

#include <stdarg.h>

#ifdef assert
#undef assert
#endif

#define LOG_TYPE_TO_ENABLE( type ) ( 1 << ( type ) )
#define LOG_TYPE_IS_ENABLED( flag , type )  ( 0 != ( ( flag ) & LOG_TYPE_TO_ENABLE( type ) ) )

namespace utl
{
    class Logger
    {
    public:
        enum LogType
        {
            LOG_TRACE,
            LOG_WARNING,
            LOG_ERROR,
            LOG_ASSERT,
            LOG_TYPE_CNT,
        };

        enum EnableType
        {
            ENABLE_TRACE   = LOG_TYPE_TO_ENABLE( LOG_TRACE ),
            ENABLE_WARNING = LOG_TYPE_TO_ENABLE( LOG_WARNING ),
            ENABLE_ERROR   = LOG_TYPE_TO_ENABLE( LOG_ERROR ),
            ENABLE_ASSERT  = LOG_TYPE_TO_ENABLE( LOG_ASSERT ),

            ENABLE_ALL  = 0xFFFFFFFF,
            ENABLE_NONE = 0,
        };

    public:
        Logger
            (
            const std::string& aName,
            EnableType aEnabledFlag  = ENABLE_ALL
            );

        void trace
            (
            const char* aFormat,
            ...
            ) const;

        void warning
            (
            const char* aFormat,
            ...
            ) const;

        void error
            (
            const char* aFormat,
            ...
            ) const;

        void assert
            (
            bool aStatement,
            const char* aFormat,
            ...
            ) const;

    private:
        void doLog
            (
            LogType aLogType,
            const char* aFormat,
            va_list& aVaList
            ) const;

    private:
        std::string iName;

        EnableType iEnabledFlag;
    };

    void trace
        (
        const char* aFormat,
        ...
        );

    void warning
        (
        const char* aFormat,
        ...
        );

    void error
        (
        const char* aFormat,
        ...
        );

    void assert
        (
        bool aStatement,
        const char* aFormat,
        ...
        );

}

#endif

