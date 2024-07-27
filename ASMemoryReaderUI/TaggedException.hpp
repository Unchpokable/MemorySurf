#pragma once

#include <string>

template <typename Tag>
class TaggedException : public std::exception {
public:
    explicit TaggedException(const std::string& message)
        : _message(message) {
    }

    const char* what() const noexcept override {
        return _message.c_str();
    }

private:
    std::string _message;
};