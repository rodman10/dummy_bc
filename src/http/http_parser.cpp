//
// Created by huang on 19-6-16.
//

#include <http/http_parser.h>

CSTR get_header_name(http_header_name header) {
    switch (header) {
#define XX(name, str) \
        bcase name: return #str;
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

CSTR http_status_str (enum http_status s)
{
    switch (s) {
#define XX(num, name, string) case HTTP_STATUS_##name: return #string;
        HTTP_STATUS_MAP(XX)
#undef XX
        default: return "<unknown>";
    }
}

http_method HttpStr2HttpEnum (CSTR method) {
    if(0) {}
#define XX(num, name, string) \
    else if(!strcmp(method,#string)) { \
        return HTTP_##name; \
    }
    HTTP_METHOD_MAP(XX)
#undef XX
    else {
        puts("unrecognized http method");
        return (http_method)-1;
    }
}

template <>
size_t HTTP_Parser<HTTP_Request>::parse(CSTR at, int length) {
    auto begin = at, end = at+length;
    for (auto ptr = at; ptr < end; ++ptr) {
        if (__IS_NEWLINE__ && (*ptr) == '\n') {
            __IS_NEWLINE__ = 0;
            ++begin;
            continue;
        }
        int len = ptr - begin;
        switch (step) {
            bcase METHOD:
                if (*(ptr) == ' ') {
                    MALLOC_CPOY_STR(data->method, begin, len);
                    step = PATH;
                    begin = ptr+1;
                }
                bcase PATH:
                if (*(ptr) == ' ') {
                    MALLOC_CPOY_STR(data->path, begin, len);
                    step=VERSION;
                    begin = ptr+1;
                }
                bcase VERSION:
                if (*(ptr) == '\r') {
                    __IS_NEWLINE__ = 1;
                    MALLOC_CPOY_STR(data->version, begin, len);
                    step=HEADERS;
                    begin = ptr+1;
                    goto END_FIRST_LINE;
                }
        }
    }
    END_FIRST_LINE:
    begin = parse_helper(begin, end);
    return begin-at;
}

template<>
HTTP_Parser<HTTP_Request>::HTTP_Parser(HTTP_Request *data):data(data) {
    step = METHOD;
}

template<>
HTTP_Parser<HTTP_Response>::HTTP_Parser(HTTP_Response *data):data(data) {
    step = VERSION;
}

template <>
size_t HTTP_Parser<HTTP_Response>::parse(CSTR at, int length) {
    auto begin = at, ptr = at, end = at+length;
    for (; ptr < end; ++ptr) {
        if (__IS_NEWLINE__ && (*ptr) == '\n') {
            __IS_NEWLINE__ = 0;
            ++begin;
            continue;
        }
        int len = ptr - begin;
        switch (step) {
            bcase VERSION:
                if (*(ptr) == ' ') {
                    MALLOC_CPOY_STR(data->version, begin, len);
                    step = CODE;
                    begin = ptr+1;
                }
                bcase CODE:
                if (*(ptr) == ' ') {
                    data->code = (http_status)str2num(begin, len);
                    step=MSG;
                    begin = ptr+1;
                }
                bcase MSG:
                if (*(ptr) == '\r') {
                    __IS_NEWLINE__ = 1;
                    MALLOC_CPOY_STR(data->msg, begin, len);
                    step=HEADERS;
                    begin = ptr+1;
                    goto END_FIRST_LINE;
                }
        }
    }
    END_FIRST_LINE:
    begin = parse_helper(begin, end);
    return begin-at;
}