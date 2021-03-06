// Copyright (c) 2012-2018 LG Electronics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

#ifndef LOG_H_
#define LOG_H_

#include <PmLogLib.h>
#include <glib.h>

enum ELogDestination
{
    eLogDestination_None = 0,

    eLogDestination_SystemLog       = 1 << 0, // System log
    eLogDestination_Terminal        = 1 << 1, // Default handler (terminal)
    eLogDestination_PrivateLogFiles = 1 << 2, // Private log files in
                                              // /var/log/<processname>.log
    eLogDestination_All = eLogDestination_SystemLog |
                          eLogDestination_Terminal |
                          eLogDestination_PrivateLogFiles
};

/// Where should log traces be sent?
// You can have them sent to more than one place at a time
void setLogDestination(ELogDestination logDestination);

/// Add log destination to the current ones
void addLogDestination(ELogDestination logDestination);

/// Set the log level. Everything higher than that & that level will be logged.
void setLogLevel(GLogLevelFlags level);
GLogLevelFlags getLogLevel(void);

void logFilter(const gchar *log_domain,
               GLogLevelFlags log_level,
               const gchar *message,
               gpointer unused_data);

/// Set the process's name, used for settings & private log file names
/// set the debug log level using the file /var/home/root/enable-<processname>-logging.
// If the file is missing, use default non-debug log level
/// if the file is empty, but present, use max log level,
//otherwise read the log level from the file's content
/// Level by numbers: 0 = error, 1 = critical, 2 = warning, 3 = message,
// 4 = info, 5 = debug, others: debug
/// Level by text: e = error, c = critical, w = warning, m = message,
// i = info, d = debug, others: debug
/// Destination: f = private log files, s = system log files,
// t = terminal. You're adding to the current destinations.
/// Note that system is enabled by default.
/// If you use private log files, only messages at message level and higher
// priority are logged in the system log (to avoid duplication).
void setProcessName(const char * name);

/// Test macro that will make a critical log entry if the test fails
#define VERIFY(t) (G_LIKELY(t) || (logFailedVerify(#t, __FILE__, __LINE__,  \
                                                        __FUNCTION__), false))

/// Test macro that will make a warning log entry if the test fails
#define CHECK(t) (G_LIKELY(t) || (logCheck(#t, __FILE__, __LINE__,  \
                                                        __FUNCTION__), false))

/// Direct critical message to put in the logs with
// file & line number, with filtering of repeats
#define FAILURE(m) logFailure(m, __FILE__, __LINE__, __FUNCTION__)

#define SHOULD_NOT_REACH_HERE FAILURE("This line should never be reached")

/// Functions that implement the macros above.
// You probably don't want to call them directly...
void logFailedVerify(const gchar * test,
                     const gchar * file,
                     int line, const gchar * function);
void logFailure(const gchar * message,
                const gchar * file,
                int line, const gchar * function);
void logCheck(const gchar * message,
              const gchar * file,
              int line, const gchar * function);

/// Helper class for log presentation purposes.
//Indents the logs with the text passed as long as the object exists.
class LogIndent {
public:
    LogIndent(const char * indent);
    ~LogIndent();

    static const char *    getTotalIndent();

private:
    const char *    mIndent;
};

#endif // LOG_H_
