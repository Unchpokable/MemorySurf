#include "pch.h"

#include "logger.h"

const std::string Logger::LogFolder = "journal";

// static functions

void Logger::panic(const std::string& what, const std::string& details) {
    critical(what, details);
    forceWrite();
    MessageBox(nullptr, details.c_str(), what.c_str(), MB_OK | MB_ICONERROR);
}

void Logger::critical(const std::string &what, const std::string &details) {
    instance().log(Level::Critical, details, what);
}

void Logger::warning(const std::string &what, const std::string &details) {
    instance().log(Level::Warning, details, what);
}

void Logger::info(const std::string &details) {
    instance().log(Level::Info, details);
}

void Logger::forceWrite() {
    auto &ins = instance();
    if(ins._bufferEnabled) {
        ins.writeBuffer();  
    }
}

void Logger::enableBuffering() {
    instance()._bufferEnabled = true;
}

void Logger::setBufferSize(std::size_t size) {
    instance()._bufferSize = size;
}

void Logger::disableBuffering() {
    instance()._bufferEnabled = false;
}

// end static

Logger::Logger(): _bufferSize(10) {
    if(!std::filesystem::exists(LogFolder)) {
        std::filesystem::create_directory(LogFolder);
    }
}

Logger::~Logger() {
    forceWrite();
}

Logger& Logger::instance() {
    static Logger logger;
    return logger;
}

void Logger::log(Level logLevel, const std::string& message, const std::string& category) {
    std::lock_guard<std::mutex> lock(_mutex);
    auto formattedMessage = formatMessage(logLevel, message, category);

    if(_bufferEnabled) {
        _buffer.emplace_back(formattedMessage);
        if(_buffer.size() >= _bufferSize) {
            writeBuffer();
        }
    } else {
        std::ofstream logFile(currentLogFile(), std::ios::app);
        logFile << formattedMessage << "\n";
        logFile.close();
    }
}

void Logger::writeBuffer() {
    if(_buffer.empty()) return;

    std::ofstream logFile(currentLogFile(), std::ios::app);
    for(const auto& message : _buffer) {
        logFile << message << '\n';
    }
    _buffer.clear();
}

std::string Logger::currentLogFile() noexcept {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
    localtime_s(&tm, &time);

    std::ostringstream oss;
    oss << LogFolder << "\\"
        << std::put_time(&tm, "%d-%m-%Y")
        << ".log";
    return oss.str();
}

std::string Logger::formatMessage(Level logLevel, const std::string& message, const std::string &category) {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
    localtime_s(&tm, &time);

    std::ostringstream oss;
    oss << "[" << std::put_time(&tm, "%d.%m.%Y") << "]";

    switch(logLevel) {
    case Level::Info: oss << "[INFO]"; break;
    case Level::Warning: oss << "[WARN]"; break;
    case Level::Critical: oss << "[CRIT]"; break;
    }

    if(!category.empty()) {
        oss << "[" << category << "]";
    }

    oss << " :: " << message;
    return oss.str();
}
