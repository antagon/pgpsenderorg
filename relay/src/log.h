#ifndef _LOG_H
#define _LOG_H

#include <syslog.h>

#define log_error(...) syslog (LOG_ERR, __VA_ARGS__)
#define log_warn(...) syslog (LOG_WARN, __VA_ARGS__)
#define log_info(...) syslog (LOG_INFO, __VA_ARGS__)

#endif

