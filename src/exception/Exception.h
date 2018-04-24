//
// Created by Milan van Zanten on 09.02.18.
//

#ifndef BESTEST_GAME_EXCEPTION_H
#define BESTEST_GAME_EXCEPTION_H

#include <string>
#include <utility>

/// General Exception
class Exception: public std::exception {
public:
    explicit Exception(const char* message, const int code = 0) : error(message), message(message), code(code) {}
    explicit Exception(std::string message, const int code = 0) : error(message), message(std::move(message)), code(code) {}
    const char* what() const throw() override {
        if(code == 0) // 0 is considered no exception code
            return (messagePrefix + messageSeparator + message).c_str();
        else
            return (messagePrefix + messageSeparator + message + " " + codePrefix + std::to_string(code)).c_str();
    }
    ~Exception() throw() override = default;

protected:
    std::runtime_error error;
    std::string messagePrefix = "Exception", codePrefix = "Error code: ", messageSeparator = ": ";

private:
    std::string message;
    /// The exception code. 0 is considered no exception code and not added to the message.
    int code = 0;
};

/// An Exception for things that have to do with IO.
class IOException: public Exception {
public:
    explicit IOException(const std::string &message, const int code = 0) : Exception(message, code) {
        messagePrefix = "IO Exception";
    };
};

/// An Exception for things that have to be set before they can be accessed.
class NotSetException: public Exception {
public:
    explicit NotSetException(const std::string &message, const int code = 0) : Exception(message + " has to be set.", code) {};
};

/// An Exception for things that have to be initialized for other things to be initialized.
class NotInitialisedException: public Exception {
public:
    explicit NotInitialisedException(const std::string &notInitialised) : Exception(notInitialised + " is not initialised.", 0) {};
    NotInitialisedException(const std::string &notInitialised, const std::string &toBeInitialised) : Exception(notInitialised + " has to be initialised before " + toBeInitialised + " can be initialised.", 0) {};
};

/// An Exception for things that are not allocated.
class NotAllocatedException: public Exception {
public:
    explicit NotAllocatedException(const std::string &notAllocated) : Exception(notAllocated + " is not allocated.", 0) {};
};

// An Exception for things that have not been created yet.
class NotCreatedException: public Exception {
public:
    explicit NotCreatedException(const std::string &notCreated) : Exception(notCreated + " has not been created yet.", 0) {};
};

#endif //BESTEST_GAME_EXCEPTION_H
