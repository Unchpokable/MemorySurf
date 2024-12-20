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

    // logs as critical() but also shows a messagebox and forces write buffer
    static void panic(const std::string& what, const std::string& details);
    static void critical(const std::string& what, const std::string& details);
    static void warning(const std::string& what, const std::string& details);
    static void info(const std::string& details);

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
