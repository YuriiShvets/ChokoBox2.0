#ifndef Logger_h
#define Logger_h
#pragma once

#include "FiloLogBuffer.h"
#include "LogLevel.h"

#include <Arduino.h>
#include <sstream>
#include <vector>
#include <ESP8266WiFi.h>

class Logger
{
public:
    static Logger &instance()
    {
        static Logger logger;
        return logger;
    }

    // Accepts Serial, Serial1, LittleFS File, WiFiClient, etc.
    void addOutput(Print &out)
    {
        if (std::find(outputs_.begin(), outputs_.end(), &out) == outputs_.end())
        {
            outputs_.push_back(&out);
        }
    }

    void removeOutput(Print &out)
    {
        outputs_.erase(std::remove(outputs_.begin(), outputs_.end(), &out), outputs_.end());
    }

    void setMinLevel(LogLevel level) { min_level_ = level; }
    LogLevel minLevel() const { return min_level_; }

    void write(const std::string &msg)
    {
        for (Print *out : outputs_)
        {
            if (out)
            {
                out->println(msg.c_str()); // Sends \r\n automatically
            }
        }
    }

private:
    Logger() : min_level_(LogLevel::DEBUG) {}
    std::vector<Print *> outputs_;
    LogLevel min_level_;
};

class LogStream
{
public:
    LogStream(LogLevel level, const char *file, int line)
    {
        // Strip directory path from __FILE__ to save RAM/Serial bandwidth
        const char *filename = strrchr(file, '/');
        filename = filename ? filename + 1 : file;

        buffer_ << "[" << millis() << "ms] "
                << "[" << levelToString(level) << "] ";
    }

    ~LogStream()
    {
        Logger::instance().write(buffer_.str());
    }

    template <typename T>
    LogStream &operator<<(const T &val)
    {
        buffer_ << val;
        return *this;
    }

    // Overload so u_int8_t type work seamlessly with <<
    LogStream &operator<<(const u_int8_t &val)
    {
        buffer_ << std::to_string(val);
        return *this;
    }

    // Overload so IPAddress objects work seamlessly with <<
    LogStream &operator<<(const IPAddress &val)
    {
        buffer_ << val.toString().c_str();
        return *this;
    }

    // Overload so Arduino String objects work seamlessly with <<
    LogStream &operator<<(const String &val)
    {
        buffer_ << val.c_str();
        return *this;
    }

private:
    std::ostringstream buffer_;

    static const char *levelToString(LogLevel l)
    {
        switch (l)
        {
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARN:
            return "WARN";
        case LogLevel::ERROR:
            return "ERROR";
        }
        return "UNKNOWN";
    }
};

#define LOG(level)                             \
    if (level < Logger::instance().minLevel()) \
        ;                                      \
    else                                       \
        LogStream(level, __FILE__, __LINE__)

#endif