/***********************************************************
 * @file        common.hpp
 * @author      wengjianhong (wengjianhong2099@163.com)
 * @brief       common function implementation
 * @version     0.1
 * @date        2023-11-15
 * @copyright   Copyright (c) 2023
 ***********************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <string>

/**
 * @brief       Create a multilevel directory.
 * @param path  directory, eg: ./a/b/c/d/e.
 * @return      0=Success, other=Fail.
*/
int CreateDirectories(const std::string& path) {
    char delimiter = '/';
    struct stat statPah;

    std::string subPath = "";
    for (int index = 0; index <= path.length(); index++) {
        subPath += path[index];

        if (path[index] == delimiter || index == path.length()) {
            if (stat(subPath.c_str(), &statPah) == 0 && S_ISDIR(statPah.st_mode)) {
                continue;
            }

            if (mkdir(subPath.c_str(), S_IRWXU | S_IRGRP | S_IROTH) != 0) {
                return false;
            }
        }
    }

    return true;
}
