/***********************************************************
 * @file        buffer.hpp
 * @author      wengjianhong (wengjianhong2099@163.com)
 * @brief       a simple buffer writter implementation
 * @version     0.1
 * @date        2023-10-26
 * @copyright   Copyright (c) 2023
 ***********************************************************/

#ifndef __BUFFER_WRITTER_H__
#define __BUFFER_WRITTER_H__


class BufferWritter {
    static const size_t     BUFFER_SIZE = 8<<20;    // »º³åÇø´óÐ¡£¬8MB
    const char*             buffer_ = nullptr;      // 


public:
    BufferWritter();
    virtual ~BufferWritter();

    void Flush();

};




#endif // !__BUFFER_WRITTER_H__
