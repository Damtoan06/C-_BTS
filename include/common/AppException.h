#ifndef APP_EXCEPTION_H
#define APP_EXCEPTION_H

#include <stdexcept>
#include <string>

class AppException : public std::runtime_error {
public:
    explicit AppException(const std::string& message)
        : std::runtime_error(message) {}
};

class DuplicateIdException : public AppException {
public:
    explicit DuplicateIdException(const std::string& id)
        : AppException("Ma dinh danh da ton tai: " + id) {}
};

class InvalidDateException : public AppException {
public:
    explicit InvalidDateException(const std::string& message)
        : AppException(message) {}
};

class ReferenceIntegrityException : public AppException {
public:
    explicit ReferenceIntegrityException(const std::string& message)
        : AppException(message) {}
};

#endif
