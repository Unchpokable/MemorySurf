#pragma once

#include <exception>
#include <string>

class InitializationError : public std::exception {
public:
    explicit InitializationError(std::string message)
        : _errorMessage(std::move(message)) {}

    virtual const char* what() const noexcept override {
        return _errorMessage.c_str();
    }

private:
    std::string _errorMessage;

};
