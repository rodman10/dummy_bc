//
// Created by huang on 19-6-9.
//

#ifndef BLOCK_CHAIN_PRECOMPILED_H
#define BLOCK_CHAIN_PRECOMPILED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CSTR const char*
#define STR char*
#define MAXLINE 1024
#define bcase break;case
#define bdefault break;default
#define malloc_str(len) (char *)malloc(((len)+1)*sizeof(char))
#define copy_str(dest, src, len) strncpy(dest, src, len), (dest)[len] = 0
#define free_ptr(ptr) free(ptr), (ptr) = nullptr
#define eb emplace_back
#define pb push_back
#define STRLEN(ptr) (ptr ? strlen(ptr) : 0)
#define JSON2Type(data, f, type) \
    data[#f].as##type();
#define JSON2VECTOR(data, f, func) \
    auto &_##f = data[#f]; \
    f.reserve(_##f.size()); \
    for (auto && e##f : _##f) { \
        f.eb(func(e##f)); \
    }
#define JSON2SET(data, f, func) \
    auto &_##f = data[#f]; \
    f.clear(); \
    for (auto && e##f : _##f) { \
        f.emplace(e##f.func()); \
    }
#define JSON2CSTR(data, f, dest) \
    auto _##f = JSON2Type(data, f, CString); \
    auto f##len = strlen(_##f); \
    dest = malloc_str(f##len); \
    copy_str(dest, (_##f), f##len);
#define Type2JSON(data, f, type) \
    data[#f] = Json::Value((type)f);
#define DEFAULTTYPE2JSON(data, f) \
    data[#f] = f;
#define VECTOR2JSON(data, f, func) \
    data[#f].append(Json::Value::null); \
    data[#f].clear(); \
    for (auto &&e##f : f) { \
        data[#f].append((e##f).func()); \
    }
#define DEFAULTVECTOR2JSON(data, f) \
    data[#f].append(Json::Value::null); \
    data[#f].clear(); \
    for (auto &&e##f : f) { \
        data[#f].append(e##f); \
    }

#define FREE(ptr) \
    if (nullptr != ptr) { \
        free(ptr); \
        ptr = nullptr; \
    }

inline int contains_char(CSTR str, int length, char c) {
    for (int i = 0; i < length; ++i) {
        if(str[i] == c) {
            return i;
        }
    }
    return -1;
}


inline int num_len(int num) {
    int len = 0;
    do {
        num /= 10;
        ++len;
    } while (num);
    return len;
}

#endif //BLOCK_CHAIN_PRECOMPILED_H
