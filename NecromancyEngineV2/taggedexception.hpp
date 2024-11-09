#pragma once

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

using LogicException = TaggedException<struct LogicExceptionTag>;
using RuntimeException = TaggedException<struct RuntimeExceptionTag>;
using ArgumentException = TaggedException<struct ArgumentExceptionTag>;
