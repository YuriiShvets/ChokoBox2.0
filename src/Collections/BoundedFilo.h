#ifndef BoundedFilo_h
#define BoundedFilo_h
#pragma once

#include <deque>
#include <stdexcept>
#include <utility>

//  TODO delete this.

template <typename T>
class BoundedFilo
{
private:
    std::size_t maxSize_;
    std::deque<T> data_;

public:
    // Accept maxSize at runtime via constructor
    explicit BoundedFilo(std::size_t maxSize) : maxSize_(maxSize)
    {
        if (maxSize_ <= 0)
        {
            throw std::invalid_argument("Max size must be greater than 0");
        }
    }

    void push(const T &value)
    {
        if (data_.size() == maxSize_)
        {
            data_.pop_back();
        }
        data_.push_front(value);
    }

    void push(T &&value)
    {
        if (data_.size() == maxSize_)
        {
            data_.pop_back();
        }
        data_.push_front(std::move(value));
    }

    void pop()
    {
        if (data_.empty())
        {
            throw std::out_of_range("BoundedFilo is empty");
        }
        data_.pop_front();
    }

    T &top()
    {
        if (data_.empty())
        {
            throw std::out_of_range("BoundedFilo is empty");
        }
        return data_.front();
    }

    const T &top() const
    {
        if (data_.empty())
        {
            throw std::out_of_range("BoundedFilo is empty");
        }
        return data_.front();
    }

    bool empty() const noexcept { return data_.empty(); }
    bool full() const noexcept { return data_.size() == maxSize_; }
    std::size_t size() const noexcept { return data_.size(); }
    std::size_t max_size() const noexcept { return maxSize_; }
    void clear() noexcept { data_.clear(); }

    auto begin() noexcept { return data_.begin(); }
    auto end() noexcept { return data_.end(); }
    auto begin() const noexcept { return data_.begin(); }
    auto end() const noexcept { return data_.end(); }
    auto cbegin() const noexcept { return data_.cbegin(); }
    auto cend() const noexcept { return data_.cend(); }
};

#endif