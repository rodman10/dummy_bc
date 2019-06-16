//
// Created by huang on 19-6-16.
//

#include <http/http.h>

CSTR get_header_name(http_header_name header) {
    switch (header) {
#define XX(name, str) \
        bcase name: printf("fdafds"); return #str;
    HEADER_NAME_MAP(XX)
#undef XX
    }
    return nullptr;
}

size_t get_header_name_length(http_header_name header) {
    switch (header) {
#define XX(name, str) \
        bcase name: return strlen(#str);
    HEADER_NAME_MAP(XX)
#undef XX
    }
    return 0;
}

CSTR get_header_value(http_header_value header) {
    switch (header) {
#define XX(name, str) \
        bcase name: return #str;
    HEADER_VALUE_MAP(XX)
#undef XX
    }
    return nullptr;
}

size_t get_header_value_length(http_header_value header) {
    switch (header) {
#define XX(name, str) \
        bcase name: return strlen(#str);
    HEADER_VALUE_MAP(XX)
#undef XX
    }
    return 0;
}