/*************************************************************
 * @file        safe_queue.hpp
 * @author      wengjianhong (wengjianhong2099@163.com)
 * @brief       a simple thread safe queue implementation.
 * @version     0.1
 * @date        2023-05-18
 * @copyright   Copyright (c) 2023
 ************************************************************/

#ifndef __SAFE_QUEUE_H__
#define __SAFE_QUEUE_H__

#include <queue>
#include <mutex>

template <typename T>
class SafeQueue
{
private:
    std::mutex          mutex_;         // a mutex is used to protect data.
    std::queue<T>       data_;          // queue data.

public:
    SafeQueue() {}
    virtual ~SafeQueue(){}

    /**
     * @brief           copy-constructed with the contents of other.
     * @param other     another container adaptor to be used as source.
    */
    SafeQueue(const SafeQueue<T>& other) {
        std::unique_lock<std::mutex> lock(mutex_);
        while (!data_.empty()) {
            data_.pop();
        }

        data_ = other.data_;
    }

    /**
     * @brief           assigns values to the container adaptor.
     * @param other     another container adaptor to be used as source.
    */
    SafeQueue<T>&  operator = (const SafeQueue<T>& other) {
        std::unique_lock<std::mutex> lock(mutex_);
        while (!data_.empty()) {
            data_.pop();
        }

        data_ = other.data_;
        return *this;
    }

    /**
     * @brief   checks whether the underlying container is empty.
     * @return  true is empty, false is not empty.
     */
    inline bool empty()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        return data_.empty();
    }

    /**
     * @brief   returns the number of elements.
     * @return  The number of elements in the container.
     */
    inline size_t size()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        return data_.size();
    }

    /**
     * @brief       access the last element.
     * @return      Reference to the first element.
    */
    inline T& front() {
        std::unique_lock<std::mutex> lock(mutex_);
        return data_.front();
    }

    /**
     * @brief       access the last element.
     * @return      Reference to the last element.
    */
    inline T& back() {
        std::unique_lock<std::mutex> lock(mutex_);
        return data_.back();
    }

    /**
     * @brief       inserts element at the end.
     * @param value arguments to forward to the constructor of the element.
     */
    inline void emplace(const T& value)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        data_.emplace(value);
    }

    /**
     * @brief       inserts element at the end.
     * @param value the value of the element to push.
     */
    inline void push(T& value)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        data_.push(value);
    }

    /**
     * @brief       Removes an element from the front of the queue.
     */
    inline void pop()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        if (!data_.empty())
        {
            data_.pop();
        }
    }
};

#endif
