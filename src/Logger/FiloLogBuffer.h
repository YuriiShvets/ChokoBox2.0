#ifndef FiloLogBuffer_h
#define FiloLogBuffer_h
#pragma once

#include <Arduino.h>
#include <algorithm>
#include <deque>

class FiloLogBuffer : public Print
{
public:
    explicit FiloLogBuffer(size_t max_entries) : max_entries_(max_entries) {}

    // Required by Arduino Print base class (handles byte-by-byte writes)
    size_t write(uint8_t c) override
    {
        if (c == '\r')
            return 1; // Ignore CR; newline triggers commit
        if (c == '\n')
        {
            commitLine();
        }
        else
        {
            current_line_ += static_cast<char>(c);
        }
        return 1;
    }

    // Fast bulk write override (used by out->println(msg.c_str()))
    size_t write(const uint8_t *buffer, size_t size) override
    {
        for (size_t i = 0; i < size; ++i)
        {
            write(buffer[i]);
        }
        return size;
    }

    // Access the stored logs (index 0 = newest, size()-1 = oldest)
    const std::deque<String> &entries() const { return entries_; }
    size_t size() const { return entries_.size(); }
    void clear() { entries_.clear(); }

private:
    size_t max_entries_;
    String current_line_;
    std::deque<String> entries_;

    void commitLine()
    {
        if (max_entries_ == 0)
        {
            current_line_ = "";
            return;
        }

        // Insert newest log at the front (FILO order)
        entries_.push_front(std::move(current_line_));
        current_line_ = "";

        // Remove the oldest entry if max size is exceeded
        if (entries_.size() > max_entries_)
        {
            entries_.pop_back();
        }
    }
};

#endif