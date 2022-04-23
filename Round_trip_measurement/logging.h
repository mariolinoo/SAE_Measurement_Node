/**
 * @file     logging.h
 * @brief    Provides simple logging functionality.
 */
#ifndef LOGGING_H_
#define LOGGING_H_

#include <cstdlib>
#include <cstring>

#include <QtCore/QDebug>

#include "constants.h"

#define __FILENAME__ (strrchr("/" __FILE__, '/') + 1)

#define MY_FUNCTION_NAME __FILENAME__ << ":" << __func__

#define _NO_SPACE .nospace()

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#define _NO_QUOTE .noquote()
#else
#define _NO_QUOTE
#endif

#define DEBUG_NO_SILENCE 4
#define INFO_NO_SILENCE  3
#define WARN_NO_SILENCE  2

// logging macros
#define DEBUG_PRINT_QDEBUG(x) qDebug() _NO_SPACE _NO_QUOTE << "[DEBUG] ("    <<  MY_FUNCTION_NAME << ":" << __LINE__ << ") " << x
#define INFO_PRINT(x) qDebug() _NO_SPACE _NO_QUOTE << "[INFO ] ("    <<  MY_FUNCTION_NAME << ":" << __LINE__ << ") " << x
#define WARN_PRINT(x) qWarning() _NO_SPACE _NO_QUOTE << "[WARN ] (" <<  MY_FUNCTION_NAME << ":" << __LINE__ << ") " << x
#define ERROR_PRINT(x) qCritical() _NO_SPACE _NO_QUOTE << "[ERROR] ("   <<  MY_FUNCTION_NAME << ":" << __LINE__ << ") " << x

#define INFO_PRINT_SILENCE(silence, x)   do { if ((silence) >= INFO_NO_SILENCE) { INFO_PRINT(x); }  } while(0)
#define WARN_PRINT_SILENCE(silence, x)   do { if ((silence) >= WARN_NO_SILENCE) { WARN_PRINT(x); }  } while(0)
#define DEBUG_PRINT_SILENCE(silence, x)  do { if ((silence) >= DEBUG_NO_SILENCE) { DEBUG_PRINT_QDEBUG(x); }  } while(0)
#define DEBUG_PRINT_SILENCE2(dbgFlag, x) do { if (dbgFlag) { DEBUG_PRINT_QDEBUG(x); }  } while(0)

#ifndef FATAL_PRINT_NO_EXIT
    #define FATAL_PRINT(x) qCritical() _NO_SPACE _NO_QUOTE << "[FATAL] ("   <<  MY_FUNCTION_NAME << ":" << __LINE__ << ") " << x; if (strstr("abc", "bc")) { exit(1); } struct fpdummy ## __LINE__
#else
    #define FATAL_PRINT(x) qCritical() _NO_SPACE _NO_QUOTE << "[FATAL] ("   <<  MY_FUNCTION_NAME << ":" << __LINE__ << ") " << x
#endif

#ifndef VERBOSE_DEBUG_PRINTING
	#define DEBUG_PRINT(x)
#else
    #define DEBUG_PRINT(x) qDebug() _NO_SPACE _NO_QUOTE << "[DEBUG] ("    <<  MY_FUNCTION_NAME << ":" << __LINE__ << ") " << x
#endif

#endif // LOGGING_H_
