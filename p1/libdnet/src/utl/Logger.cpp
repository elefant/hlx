#include <vector>
#include <stdio.h>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/conversion.hpp>
#include <stdio.h>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include "Logger.h"
#include "String.h"

namespace utl
{
    /**
     * Ctor
     * @param aName
     */
    Logger::Logger
        (
        const std::string& aName,
        EnableType aEnabledFlag
        )
        : iName( aName )
        , iEnabledFlag( aEnabledFlag )
    {
        setlinebuf( stdout );
    }

    /**
     * The only one point to output the message
     * @param aLogType
     * @param aFormat
     * @param aVaList
     */
    void Logger::doLog
        (
        LogType aLogType,
        const char* aFormat,
        va_list& aVaList
        ) const
    {
        if( !LOG_TYPE_IS_ENABLED( iEnabledFlag, aLogType ) )
        {
            return;
        }

        std::string msg = strFormat( aFormat, aVaList );

        std::string typeString;
        switch( aLogType )
        {
            case LOG_TRACE:
                typeString = "TRACE";
                break;

            case LOG_WARNING:
                typeString = "WARNING";
                break;

            case LOG_ERROR:
                typeString = "ERROR";
                break;

            case LOG_ASSERT:
                typeString = "ASSERT";
                break;

            default: break;
        }

        std::string formattedLog = strFormat
            (
            "%s|%s|%s\n",
            iName.c_str(),
            typeString.c_str(),
            msg.c_str()
            );

        std::cout << formattedLog;
    }

    /**
     * Trace
     * @param aFormat
     * @param aVaList
     */
    void Logger::trace
        (
        const char* aFormat,
        ...
        ) const
    {
        va_list aVaList;
        va_start( aVaList, aFormat );
        doLog( LOG_TRACE, aFormat, aVaList );
        va_end( aVaList );
    }

    /**
     * Warning
     * @param aFormat
     * @param aVaList
     */
    void Logger::warning
        (
        const char* aFormat,
        ...
        ) const
    {
        va_list aVaList;
        va_start( aVaList, aFormat );
        va_end( aVaList );
        doLog( LOG_WARNING, aFormat, aVaList );
    }

    /**
     * error
     * @param aFormat
     * @param aVaList
     */
    void Logger::error
        (
        const char* aFormat,
        ...
        ) const
    {
        va_list aVaList;
        va_start( aVaList, aFormat );
        va_end( aVaList );
        doLog( LOG_ERROR, aFormat, aVaList );
    }

    /**
     * Assert
     * @param aFormat
     * @param aVaList
     */
    void Logger::assert
        (
        bool aStatement,
        const char* aFormat,
        ...
        ) const
    {
        if( false == aStatement )
        {
            va_list aVaList;
            va_start( aVaList, aFormat );
            va_end( aVaList );
            doLog( LOG_ASSERT, aFormat, aVaList );
        }
    }
}

