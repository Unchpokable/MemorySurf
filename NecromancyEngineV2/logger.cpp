#include "pch.h"

#include "logger.h"
#include "version.h"

const std::string Logger::_logDirectory = "journal";

std::string operator*(const std::string& str, int repeat)
{
    if(repeat <= 0) {
        return "";
    }

    std::string result;
    result.reserve(str.size() * repeat);
    for(int i = 0; i < repeat; ++i) {
        result += str;
    }

    return result;
}

// static functions

void Logger::panic(const std::string& what, const std::string& details)
{
    critical(what, details);
    forceWrite();

    std::stringstream panicMessage;

    panicMessage << "what happened: " << what << "\n Details: " << details;

    MessageBox(nullptr, panicMessage.str().c_str(), "MemorySurf has stopped working due to critical error!", MB_OK | MB_ICONERROR);
}

void Logger::critical(const std::string& what, const std::string& details)
{
    instance().log(Level::Critical, details, what);
}

void Logger::warning(const std::string& what, const std::string& details)
{
    instance().log(Level::Warning, details, what);
}

void Logger::info(const std::string& details)
{
    instance().log(Level::Info, details);
}

void Logger::forceWrite()
{
    auto& ins = instance();
    if(ins._bufferEnabled) {
        ins.writeBuffer();
    }
}

void Logger::enableBuffering()
{
    instance()._bufferEnabled = true;
}

void Logger::setBufferSize(std::size_t size)
{
    instance()._bufferSize = size;
}

void Logger::disableBuffering()
{
    instance()._bufferEnabled = false;
}

// end static

Logger::Logger(): _bufferSize(10)
{
    if(!std::filesystem::exists(_logDirectory)) {
        std::filesystem::create_directory(_logDirectory);
    }

    std::stringstream initMessage;
    initMessage << std::string("=") * 15 << "\n";
    initMessage << "MemorySurf logs session\n";
    initMessage << "'MemorySurf' Memory Reader runs on NecromancyEngine v2. Author : unchpokable\n";
    initMessage << "contact author: https://github.com/Unchpokable\n";
    initMessage << "build: " << completeVersion << "\n";
    initMessage << std::string("=") * 15 << "\n";

    std::ofstream log(currentLogFile(), std::ios::app);
    log << initMessage.str();
    log.close();
}

Logger::~Logger()
{
    forceWrite();
}

Logger& Logger::instance()
{
    static Logger logger;
    return logger;
}

void Logger::log(Level logLevel, const std::string& message, const std::string& category)
{
    std::lock_guard<std::mutex> lock(_mutex);
    auto formattedMessage = formatMessage(logLevel, message, category);

    if(_bufferEnabled) {
        _buffer.emplace_back(formattedMessage);
        if(_buffer.size() >= _bufferSize) {
            writeBuffer();
        }
    }
    else {
        std::ofstream logFile(currentLogFile(), std::ios::app);
        logFile << formattedMessage << "\n";
        logFile.close();
    }
}

void Logger::writeBuffer()
{
    if(_buffer.empty()) return;

    std::ofstream logFile(currentLogFile(), std::ios::app);
    for(const auto& message : _buffer) {
        logFile << message << '\n';
    }
    _buffer.clear();
}

std::string Logger::currentLogFile() noexcept
{
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
    localtime_s(&tm, &time);

    std::ostringstream oss;
    oss << _logDirectory << "\\"
    << std::put_time(&tm, "%d-%m-%Y")
    << ".log";
    return oss.str();
}

std::string Logger::formatMessage(Level logLevel, const std::string& message, const std::string& category)
{
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
    localtime_s(&tm, &time);

    std::ostringstream oss;
    oss << "[" << std::put_time(&tm, "%d.%m.%Y") << "]";

    switch(logLevel) {
        case Level::Info: oss << "[INFO]";
            break;
        case Level::Warning: oss << "[WARN]";
            break;
        case Level::Critical: oss << "[CRIT]";
            break;
    }

    if(!category.empty()) {
        oss << "[" << category << "]";
    }

    oss << " :: " << message;
    return oss.str();
}
