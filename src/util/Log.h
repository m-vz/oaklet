//
// Created by Milan van Zanten on 09.02.18.
//

#ifndef BESTEST_GAME_LOG_H
#define BESTEST_GAME_LOG_H

#include <string>
#include <fstream>
#include "Config.h"

enum LogType {
    LOG_ERROR = 0,
    LOG_WARNING = 1,
    LOG_INFO = 2,
    LOG_DEBUG = 3
};

class FileLog {
public:
    /// Create a logger that logs to a file. Only logs messages with a severity <= logLevel.
    /// \param logPath The file to log to, defaults to DEFAULT_LOG_PATH.
    /// \param logLevel The log severity level, defaults to LOG_WARNING.
    FileLog(const std::string logPath = CONFIG.DEFAULT_LOG_FILE_PATH, const LogType logLevel = (const LogType) CONFIG.DEFAULT_FILE_LOG_LEVEL);
    FileLog(const FileLog &) = delete; // Prevent copies.
    FileLog& operator=(const FileLog &) = delete; // Prevent copies.
    friend FileLog& operator<<(FileLog &log, const LogType type);
    friend FileLog& operator<<(FileLog &log, const char *message);
    friend FileLog& operator<<(FileLog &log, const std::string message);
    friend FileLog& operator<<(FileLog &log, const int message);
    friend FileLog& operator<<(FileLog &log, const float message);
    virtual ~FileLog();
    void setLogLevel(LogType logLevel);

private:
    LogType logLevel;
    std::ofstream logFile;
    unsigned int count[2]{};
    int currentLevel = 0;
    bool previousWasMessage = false;
};

class Log {
public:
    static Log log;

    /// Create a logger that logs to cout or cerr. Only logs messages with a severity <= logLevel. Messages with a severity of LOG_WARNING or higher are sent to cerr.
    /// \param logLevel The log severity level, defaults to LOG_WARNING.
    /// \param logToFile If true, this logger also logs to a file using a FileLog.
    /// \param fileLogPath If logToFile is true, this path is used to create the file logger.
    /// \param fileLogLevel If logToFile is true, this log level is used to create the file logger.
    Log(const LogType logLevel = (const LogType) CONFIG.DEFAULT_LOG_LEVEL,
        const bool logToFile = false,
        const std::string fileLogPath = CONFIG.DEFAULT_LOG_FILE_PATH,
        const LogType fileLogLevel = (const LogType) CONFIG.DEFAULT_FILE_LOG_LEVEL);
    Log(const Log &) = delete; // Prevent copies.
    Log& operator=(const Log &) = delete; // Prevent copies.
    friend Log& operator<<(Log &log, const LogType type);
    friend Log& operator<<(Log &log, const char *message);
    friend Log& operator<<(Log &log, const std::string message);
    friend Log& operator<<(Log &log, const int message);
    friend Log& operator<<(Log &log, const long int message);
    friend Log& operator<<(Log &log, const long long int message);
    friend Log& operator<<(Log &log, const float message);
    friend Log& operator<<(Log &log, const double message);
    virtual ~Log();
    void setLogLevel(LogType logLevel, bool setForFileLog = true);

private:
    bool logToFile;
    FileLog *fileLog;
    LogType logLevel;
    int currentLevel = 0;
    bool previousWasMessage = false;
};

#endif //BESTEST_GAME_LOG_H
