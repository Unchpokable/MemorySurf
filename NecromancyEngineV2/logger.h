#pragma once

class Logger final {
public:
    enum class Level {
        Critical,
        Warning,
        Info
    };

    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;

    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&) = delete;

    /// @brief logs as \c Logger::critical() but also shows a \c MessageBox and forces write buffer
    static void panic(const std::string& what, const std::string& details);
    static void critical(const std::string& what, const std::string& details);
    static void warning(const std::string& what, const std::string& details);
    static void info(const std::string& details);

    template<typename Cond>
    static void logCondition(Cond condition, const std::string& shortDescription, const std::string& fullDescription = "");

    static void forceWrite();

    static void enableBuffering();
    static void setBufferSize(std::size_t size);
    static void disableBuffering();

private:
    Logger();
    ~Logger();

    static Logger& instance();

    void log(Level logLevel, const std::string& message, const std::string& category = "");
    void writeBuffer();

    static std::string currentLogFile() noexcept;
    static std::string formatMessage(Level logLevel, const std::string& message, const std::string& category = "");

    static const std::string LogFolder;

    // buffering
    std::size_t _bufferSize;
    std::vector<std::string> _buffer {};
    bool _bufferEnabled { true };

    // other
    std::mutex _mutex {};
};

template<typename Cond>
void Logger::logCondition(Cond condition, const std::string& shortDescription, const std::string& fullDescription) {
    auto result { false };

    if constexpr(std::is_invocable_r_v<bool, Cond>) {
        result = condition();
    } else if constexpr(std::is_same_v<bool, Cond>) {
        result = condition;
    }

    std::string fullMessage;
    if(result) {
        fullMessage = "Condition OK: " + shortDescription + ", " + fullDescription;
        info(fullMessage);
    } else {
        fullMessage = "Condition FAIL: " + shortDescription + ", " + fullDescription;
        warning("COND CHECK", fullMessage);
    }
}
