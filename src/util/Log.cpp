//
// Created by Milan van Zanten on 09.02.18.
//

#include <iostream>
#include "Log.h"
#include "../exception/Exception.h"

Log Log::log((const LogType) CONFIG.DEFAULT_LOG_LEVEL, true); // NOLINT

FileLog::FileLog(const std::string logPath, const LogType logLevel) : logLevel(logLevel) {
    logFile.open(logPath);

    if(!logFile.is_open()) {
        std::cerr << "Could not open log file: " + logPath << std::endl;
        throw IOException("Could not open log file: " + logPath);
    }
}

FileLog& operator<<(FileLog &log, const LogType type) {
    if(log.previousWasMessage && log.logLevel >= log.currentLevel)
        log.logFile << std::endl;

    switch(type) {
        case LOG_ERROR:
            if(log.logLevel >= LOG_ERROR) {
                log.count[LOG_ERROR]++;
                log.logFile << "ERROR: ";
            }
            break;
        case LOG_WARNING:
            if(log.logLevel >= LOG_WARNING) {
                log.count[LOG_WARNING]++;
                log.logFile << "WARNING: ";
            }
            break;
        case LOG_INFO:
            if(log.logLevel >= LOG_INFO)
                log.logFile << "INFO: ";
            break;
        case LOG_DEBUG:
            if(log.logLevel >= LOG_DEBUG)
                log.logFile << "DEBUG: ";
            break;
        case LOG_FRAME:
            if(log.logLevel >= LOG_FRAME)
                log.logFile << "FRAME: ";
            break;
    }

    log.currentLevel = type;
    log.previousWasMessage = false;
    return log;
}

FileLog& operator<<(FileLog &log, const char *message) {
    if(log.logLevel >= log.currentLevel)
        log.logFile << message;

    log.previousWasMessage = true;
    return log;
}

FileLog::~FileLog() {
    if(logFile.is_open()) {
        logFile << "\n\n" << count[LOG_ERROR] << " errors\n" << count[LOG_WARNING] << " warnings";
        logFile.close();
    }
}

void FileLog::setLogLevel(LogType logLevel) {
    FileLog::logLevel = logLevel;
}

Log::Log(const LogType logLevel,
         const bool logToFile,
         const std::string fileLogPath,
         const LogType fileLogLevel) : logLevel(logLevel), logToFile(logToFile) {
    if(logToFile)
        fileLog = new FileLog(fileLogPath, fileLogLevel);
}

Log &operator<<(Log &log, const LogType type) {
    if(log.previousWasMessage && log.logLevel >= log.currentLevel) {
        if(type == LOG_ERROR || type == LOG_WARNING)
            std::cerr << std::endl;
        else if(type == LOG_INFO || type == LOG_DEBUG || type == LOG_FRAME)
            std::cout << std::endl;
    }

    switch(type) {
        case LOG_ERROR:
            if(log.logLevel >= LOG_ERROR)
                std::cerr << "ERROR: ";
            break;
        case LOG_WARNING:
            if(log.logLevel >= LOG_WARNING)
                std::cerr << "WARNING: ";
            break;
        case LOG_INFO:
            if(log.logLevel >= LOG_INFO)
                std::cout << "INFO: ";
            break;
        case LOG_DEBUG:
            if(log.logLevel >= LOG_DEBUG)
                std::cout << "DEBUG: ";
            break;
        case LOG_FRAME:
            if(log.logLevel >= LOG_FRAME)
                std::cout << "FRAME: ";
            break;
        default:
            break;
    }

    if(log.logToFile)
        *log.fileLog << type;

    log.currentLevel = type;
    log.previousWasMessage = false;
    return log;
}

Log &operator<<(Log &log, const char *message) {
    switch(log.currentLevel) {
        case LOG_ERROR:
            if(log.logLevel >= LOG_ERROR)
                std::cerr << message;
            break;
        case LOG_WARNING:
            if(log.logLevel >= LOG_WARNING)
                std::cerr << message;
            break;
        case LOG_INFO:
            if(log.logLevel >= LOG_INFO)
                std::cout << message;
            break;
        case LOG_DEBUG:
            if(log.logLevel >= LOG_DEBUG)
                std::cout << message;
            break;
        case LOG_FRAME:
            if(log.logLevel >= LOG_FRAME)
                std::cout << message;
            break;
        default:
            break;
    }

    if(log.logToFile)
        *log.fileLog << message;

    log.previousWasMessage = true;
    return log;
}

Log &operator<<(Log &log, const std::string message) {
    return log << message.c_str();
}

Log &operator<<(Log &log, const char message) {
    return log << std::to_string(message).c_str();
}

Log &operator<<(Log &log, const unsigned char message) {
    return log << std::to_string(message).c_str();
}

Log &operator<<(Log &log, const int message) {
    return log << std::to_string(message).c_str();
}

Log &operator<<(Log &log, const unsigned int message) {
    return log << std::to_string(message).c_str();
}

Log &operator<<(Log &log, const long int message) {
    return log << std::to_string(message).c_str();
}

Log &operator<<(Log &log, const unsigned long int message) {
    return log << std::to_string(message).c_str();
}

Log &operator<<(Log &log, const long long int message) {
    return log << std::to_string(message).c_str();
}

Log &operator<<(Log &log, const unsigned long long int message) {
    return log << std::to_string(message).c_str();
}

Log &operator<<(Log &log, const float message) {
    return log << std::to_string(message).c_str();
}

Log &operator<<(Log &log, const double message) {
    return log << std::to_string(message).c_str();
}

Log &operator<<(Log &log, const glm::vec3 message) {
    return log << ("(" + std::to_string(message.x) + ", " + std::to_string(message.y) + ", " + std::to_string(message.z) + ")").c_str();
}

void Log::setLogLevel(LogType logLevel, bool setForFileLog) {
    Log::logLevel = logLevel;

    if(setForFileLog && logToFile)
        fileLog->setLogLevel(logLevel);
}

Log::~Log() {
    delete fileLog;
}
