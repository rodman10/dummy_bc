//
// Created by huang on 19-6-20.
//

#ifndef BLOCK_CHAIN_HTTP_PARSER_H
#define BLOCK_CHAIN_HTTP_PARSER_H
#include <util/precompiled.h>
#include <netinet/in.h>

#define BLANK_LINE 2

#define SERVER_STRING "Server: httpd/0.1.0\r\n"
#define BASIC_REQUEST_STR_LEN(method, path) \
    strlen("  HTTP/1.1\r\n") + strlen(method) + STRLEN(path) + 1

#define BASIC_REQUEST_STR(buf, method, path) \
    sprintf(buf, "%s %s HTTP/1.1\r\n", method, STRLEN(path) ? (path) : "/"); \

#define BASIC_RESPONSE_STR_LEN(code) \
    strlen("HTTP/1.1  \r\n") + num_len(code) + strlen(http_status_str(code))

#define BASIC_RESPONSE_STR(buf, code) \
    sprintf(buf, "HTTP/1.1 %d %s\r\n", \
            code, http_status_str(code));

#define SERIALIZE_HEADERS(buf) \
    for (int i = 0; i < headers_size; ++i) { \
        sprintf(buf,"%s%s: %s\r\n", buf, headers[i][0], headers[i][1]); \
    } \
    sprintf(buf, "%s\r\n", buf);

#define SERIALIZE_BODY(buf) \
    if (body_length) { \
        sprintf(buf, "%s%s", buf, body); \
    }

// use hash to find header
#define GET_HEADER(request, name, dest) \
    STR dest; \
    for (int i = 0; i < request.headers_size; i++) { \
        if (!strcmp(request.headers[i][0], get_header_name(name))) { \
            dest = request.headers[i][1]; \
            break; \
        } \
    }

#define ADD_HEADER(name, value) \
    AddHeaderKey(get_header_name(name), get_header_name_length(name)); \
    AddHeaderValue(get_header_value(value), get_header_value_length(value)); \

#define ADD_HEADER_WITH_OBJ(obj, name, value) \
    obj.AddHeaderKey(get_header_name(name), get_header_name_length(name)); \
    obj.AddHeaderValue(get_header_value(value), get_header_value_length(value)); \


#define HEADER_NAME_MAP(XX) \
    XX(CONTENT_TYPE, Content-Type) \
    XX(CONTENT_LENGTH, Content-Length) \
    XX(CONNECTION, Connection) \
    XX(HOST, Host) \

enum http_header_name {
#define XX(name, string) name,
    HEADER_NAME_MAP(XX)
#undef XX
};

CSTR get_header_name(http_header_name header);

size_t get_header_name_length(http_header_name header);


#define HEADER_VALUE_MAP(XX) \
    XX(APPLICATION_JSON, application/json) \
    XX(TEXT_PLAIN, text/plain) \
    XX(CLOSE, close) \
    XX(KEEP_ALIVE, keep-alive) \

enum http_header_value {
#define XX(name, string) name,
    HEADER_VALUE_MAP(XX)
#undef XX
};

CSTR get_header_value(http_header_value header);

size_t get_header_value_length(http_header_value header);

/* Status Codes */
#define HTTP_STATUS_MAP(XX)                                                 \
  XX(100, CONTINUE,                        Continue)                        \
  XX(101, SWITCHING_PROTOCOLS,             Switching Protocols)             \
  XX(102, PROCESSING,                      Processing)                      \
  XX(200, OK,                              OK)                              \
  XX(201, CREATED,                         Created)                         \
  XX(202, ACCEPTED,                        Accepted)                        \
  XX(203, NON_AUTHORITATIVE_INFORMATION,   Non-Authoritative Information)   \
  XX(204, NO_CONTENT,                      No Content)                      \
  XX(205, RESET_CONTENT,                   Reset Content)                   \
  XX(206, PARTIAL_CONTENT,                 Partial Content)                 \
  XX(207, MULTI_STATUS,                    Multi-Status)                    \
  XX(208, ALREADY_REPORTED,                Already Reported)                \
  XX(226, IM_USED,                         IM Used)                         \
  XX(300, MULTIPLE_CHOICES,                Multiple Choices)                \
  XX(301, MOVED_PERMANENTLY,               Moved Permanently)               \
  XX(302, FOUND,                           Found)                           \
  XX(303, SEE_OTHER,                       See Other)                       \
  XX(304, NOT_MODIFIED,                    Not Modified)                    \
  XX(305, USE_PROXY,                       Use Proxy)                       \
  XX(307, TEMPORARY_REDIRECT,              Temporary Redirect)              \
  XX(308, PERMANENT_REDIRECT,              Permanent Redirect)              \
  XX(400, BAD_REQUEST,                     Bad Request)                     \
  XX(401, UNAUTHORIZED,                    Unauthorized)                    \
  XX(402, PAYMENT_REQUIRED,                Payment Required)                \
  XX(403, FORBIDDEN,                       Forbidden)                       \
  XX(404, NOT_FOUND,                       Not Found)                       \
  XX(405, METHOD_NOT_ALLOWED,              Method Not Allowed)              \
  XX(406, NOT_ACCEPTABLE,                  Not Acceptable)                  \
  XX(407, PROXY_AUTHENTICATION_REQUIRED,   Proxy Authentication Required)   \
  XX(408, REQUEST_TIMEOUT,                 Request Timeout)                 \
  XX(409, CONFLICT,                        Conflict)                        \
  XX(410, GONE,                            Gone)                            \
  XX(411, LENGTH_REQUIRED,                 Length Required)                 \
  XX(412, PRECONDITION_FAILED,             Precondition Failed)             \
  XX(413, PAYLOAD_TOO_LARGE,               Payload Too Large)               \
  XX(414, URI_TOO_LONG,                    URI Too Long)                    \
  XX(415, UNSUPPORTED_MEDIA_TYPE,          Unsupported Media Type)          \
  XX(416, RANGE_NOT_SATISFIABLE,           Range Not Satisfiable)           \
  XX(417, EXPECTATION_FAILED,              Expectation Failed)              \
  XX(421, MISDIRECTED_REQUEST,             Misdirected Request)             \
  XX(422, UNPROCESSABLE_ENTITY,            Unprocessable Entity)            \
  XX(423, LOCKED,                          Locked)                          \
  XX(424, FAILED_DEPENDENCY,               Failed Dependency)               \
  XX(426, UPGRADE_REQUIRED,                Upgrade Required)                \
  XX(428, PRECONDITION_REQUIRED,           Precondition Required)           \
  XX(429, TOO_MANY_REQUESTS,               Too Many Requests)               \
  XX(431, REQUEST_HEADER_FIELDS_TOO_LARGE, Request Header Fields Too Large) \
  XX(451, UNAVAILABLE_FOR_LEGAL_REASONS,   Unavailable For Legal Reasons)   \
  XX(500, INTERNAL_SERVER_ERROR,           Internal Server Error)           \
  XX(501, NOT_IMPLEMENTED,                 Not Implemented)                 \
  XX(502, BAD_GATEWAY,                     Bad Gateway)                     \
  XX(503, SERVICE_UNAVAILABLE,             Service Unavailable)             \
  XX(504, GATEWAY_TIMEOUT,                 Gateway Timeout)                 \
  XX(505, HTTP_VERSION_NOT_SUPPORTED,      HTTP Version Not Supported)      \
  XX(506, VARIANT_ALSO_NEGOTIATES,         Variant Also Negotiates)         \
  XX(507, INSUFFICIENT_STORAGE,            Insufficient Storage)            \
  XX(508, LOOP_DETECTED,                   Loop Detected)                   \
  XX(510, NOT_EXTENDED,                    Not Extended)                    \
  XX(511, NETWORK_AUTHENTICATION_REQUIRED, Network Authentication Required) \

enum http_status
{
#define XX(num, name, string) HTTP_STATUS_##name = num,
    HTTP_STATUS_MAP(XX)
#undef XX
};

CSTR http_status_str (enum http_status s);

/* Request Methods */
#define HTTP_METHOD_MAP(XX)         \
  XX(0,  DELETE,      DELETE)       \
  XX(1,  GET,         GET)          \
  XX(2,  HEAD,        HEAD)         \
  XX(3,  POST,        POST)         \
  XX(4,  PUT,         PUT)          \
  /* pathological */                \
  XX(5,  CONNECT,     CONNECT)      \
  XX(6,  OPTIONS,     OPTIONS)      \
  XX(7,  TRACE,       TRACE)        \
  /* WebDAV */                      \
  XX(8,  COPY,        COPY)         \
  XX(9,  LOCK,        LOCK)         \
  XX(10, MKCOL,       MKCOL)        \
  XX(11, MOVE,        MOVE)         \
  XX(12, PROPFIND,    PROPFIND)     \
  XX(13, PROPPATCH,   PROPPATCH)    \
  XX(14, SEARCH,      SEARCH)       \
  XX(15, UNLOCK,      UNLOCK)       \
  XX(16, BIND,        BIND)         \
  XX(17, REBIND,      REBIND)       \
  XX(18, UNBIND,      UNBIND)       \
  XX(19, ACL,         ACL)          \
  /* subversion */                  \
  XX(20, REPORT,      REPORT)       \
  XX(21, MKACTIVITY,  MKACTIVITY)   \
  XX(22, CHECKOUT,    CHECKOUT)     \
  XX(23, MERGE,       MERGE)        \
  /* upnp */                        \
  XX(24, MSEARCH,     M-SEARCH)     \
  XX(25, NOTIFY,      NOTIFY)       \
  XX(26, SUBSCRIBE,   SUBSCRIBE)    \
  XX(27, UNSUBSCRIBE, UNSUBSCRIBE)  \
  /* RFC-5789 */                    \
  XX(28, PATCH,       PATCH)        \
  XX(29, PURGE,       PURGE)        \
  /* CalDAV */                      \
  XX(30, MKCALENDAR,  MKCALENDAR)   \
  /* RFC-2068, section 19.6.1.2 */  \
  XX(31, LINK,        LINK)         \
  XX(32, UNLINK,      UNLINK)       \
  /* icecast */                     \
  XX(33, SOURCE,      SOURCE)

enum http_method
{
#define XX(num, name, string) HTTP_##name = num,
    HTTP_METHOD_MAP(XX)
#undef XX
};

http_method HttpStr2HttpEnum (CSTR method);


struct HTTP_Headers {
    size_t headers_size=0;
    STR (*headers)[2]= nullptr;
    size_t headers_length=0;

    void AddHeaderKey(CSTR at, size_t length) {
        auto tmp = (char* (*)[2])realloc(headers, (1+headers_size)* sizeof(headers[0]));
        // TODO realloc failure
        if (!tmp) {
            return;
        }
        headers = tmp;
        headers[headers_size][0] = MALLOC_STR(length);
        COPY_STR(headers[headers_size][0], at, length);
        // why plus 2: ':' and space
        headers_length += length + 2;
    }
    void AddHeaderValue(CSTR at, size_t length) {
        headers[headers_size][1] = MALLOC_STR(length);
        COPY_STR(headers[headers_size][1], at, length);
        // why plus 2: \r\n
        headers_length += length + 2;
        ++headers_size;
    }

    HTTP_Headers() {}

    HTTP_Headers(const HTTP_Headers &_headers) {
        headers_length = _headers.headers_length;
        headers = _headers.headers;
        headers_size = _headers.headers_size;
    }

    virtual ~HTTP_Headers() {
        if (headers) {
            for (int i = 0; i < headers_size; ++i) {
                FREE(headers[i][0]);
                FREE(headers[i][1]);
            }
            FREE(headers);
        }
    }
};

struct HTTP_Base : HTTP_Headers {
    STR version = nullptr;
    STR hostname = nullptr;
    STR port = nullptr;
    STR body = nullptr;
    size_t body_length = 0;

    HTTP_Base() {}

    HTTP_Base(HTTP_Headers &headers) : HTTP_Headers(headers) {}

    ~HTTP_Base() {

        FREE(body);
    }

    virtual STR Serialize() const = 0;

    int AppendBody(CSTR at, int pos, size_t length) {
        COPY_STR(body+pos, at, length);
        return pos + length;
    }

    void SetBody(CSTR at, size_t length) {
        body = MALLOC_STR(length);
        COPY_STR(body, at, length);
    }
};

struct HTTP_Request: HTTP_Base {

    enum STEP {
        SCHEME,
        HOSTNAME,
        PORT,
        PATH,
    };

    STR path;
    STR method;

    void set_hostname(CSTR at, size_t length) {
        hostname = MALLOC_STR(length);
        COPY_STR(hostname, at, length);
    }

    void set_port(CSTR at, size_t length) {
        port = MALLOC_STR(length);
        COPY_STR(port, at, length);
    }

    void set_path(CSTR at, size_t length) {
        path = MALLOC_STR(length);
        COPY_STR(path, at, length);
    }

    HTTP_Request(CSTR url, HTTP_Headers &headers) : HTTP_Base(headers) {
        auto length = strlen(url);
        auto n = strlen(url);
        STEP step = SCHEME;
        auto ptr = url, begin = url, end = url+n;
        CSTR host_st = nullptr;
        CSTR host_ed= nullptr;
        for (; ptr <= end; ++ptr) {
            auto len = ptr-begin;
            switch (step) {
                bcase SCHEME:
                    if ((*ptr)==':') {
                        ptr += 2;
                        begin = ptr+1;
                        host_st = begin;
                        step = HOSTNAME;
                    }
                    bcase HOSTNAME:
                    if ((*ptr)=='/') {
                        set_hostname(begin, len);
                        begin = ptr;
                        host_ed = ptr;
                        step = PATH;
                    } else if ((*ptr) == ':') {
                        set_hostname(begin, len);
                        begin = ptr+1;
                        step = PORT;
                    }
                    bcase PORT:
                    if ((*ptr) == '/') {
                        set_port(begin, len);
                        begin = ptr;
                        host_ed = ptr;
                        step = PATH;
                    }
                    bcase PATH:
                    if (ptr == end) {
                        set_path(begin, len);
                    }
            }
        }
        if (!port) {
            set_port("80", 2);
        }
        AddHeaderKey(get_header_name(HOST), get_header_name_length(HOST));

        AddHeaderValue(host_st, host_ed-host_st);
    }

    HTTP_Request() {}

    STR Serialize() const {
        // why plus 2: between header and body
        STR buf = MALLOC_STR(BASIC_REQUEST_STR_LEN(method, path) + headers_length + BLANK_LINE + body_length);
        BASIC_REQUEST_STR(buf, method, path);

        SERIALIZE_HEADERS(buf);

        SERIALIZE_BODY(buf);

        return buf;
    }

    // shallow copy, RVO trick
    ~HTTP_Request() {
        FREE(hostname);
        FREE(path);
        FREE(port);
    }
};


//TODO add keep alive
struct HTTP_Response : HTTP_Base {
    http_status code;
    STR msg;

    HTTP_Response() {}

    HTTP_Response(http_status code) : code(code) {}
    HTTP_Response(HTTP_Response &&response) {
        std::swap(headers_size, response.headers_size);
        std::swap(headers_length, response.headers_length);
        std::swap(headers, response.headers);
        std::swap(body, response.body);
        std::swap(body_length, response.body_length);
        std::swap(hostname, response.hostname);
        std::swap(port, response.port);
        std::swap(version, response.version);
        std::swap(code, response.code);
        std::swap(msg, response.msg);
    }

    HTTP_Response(CSTR body, http_header_value body_type, http_status code) : code(code) {
        headers = nullptr;
        auto len = STRLEN(body);
        body_length = len;
        if (len) {
            this->body = MALLOC_STR(len);
            COPY_STR(this->body, body, len);
        }
        // maybe add keep alive
        ADD_HEADER(CONNECTION, CLOSE);
        ADD_HEADER(CONTENT_TYPE, body_type);
        AddHeaderKey(get_header_name(CONTENT_LENGTH), get_header_name_length(CONTENT_LENGTH));
        auto n_len = num_len(len);
        STR buf = MALLOC_STR(n_len);
        sprintf(buf, "%zu", len);
        AddHeaderValue(buf, n_len);
        FREE(buf);
    }

    STR Serialize() const {
        STR buf = MALLOC_STR(BASIC_RESPONSE_STR_LEN(code) + headers_length + BLANK_LINE + body_length);
        BASIC_RESPONSE_STR(buf, code);

        SERIALIZE_HEADERS(buf);

        SERIALIZE_BODY(buf);

        return buf;
    }

    ~HTTP_Response() {

    }
};

template <typename T>
struct HTTP_Parser {
    enum STEP {
        METHOD,
        PATH,
        VERSION,
        HEADERS,
        BODY,
        CODE,
        MSG,
    };

    STEP step;
    T *data;
    bool __EOF__ = 0;
    bool __IS_NEWLINE__ = 0;
    bool __EXPECT_HEADER_VALUE__ = 0;
    bool __IS_CONTENT_LENGTH__ = 0;
    bool __IS_HOST__ = 0;
    size_t body_offset = 0;

    HTTP_Parser(T *data);

    bool Parse(int client) {
        size_t bytes_received, tot_bytes_received = 0;
        size_t parsed_bytes = 0, start = 0;
        int buf_size = 16;
        STR buf = (STR)malloc(sizeof(char) * buf_size);
        while (!__EOF__ && (bytes_received = recv(client, buf+start, buf_size-start, 0))) {
            parsed_bytes = parse(buf, bytes_received + start);
            if (!parsed_bytes) {
                start = buf_size;
                buf_size <<= 1;
                auto tmp = (STR)realloc(buf, sizeof(char)*buf_size);
                if (!tmp) {
                    puts("realloc memory failed");
                }
                buf = tmp;
            } else {
                start = buf_size -parsed_bytes;
                strncpy(buf, buf+parsed_bytes, start);
            }
        }
        FREE(buf);
    }
    
    size_t parse(CSTR at, int length);

    CSTR parse_helper(CSTR at, CSTR end) {
        auto begin = at;
        for(auto ptr = at; ptr < end; ++ptr) {
            if (__IS_NEWLINE__ && (*ptr) == '\n') {
                __IS_NEWLINE__ = 0;
                ++begin;
                continue;
            }
            int len = ptr - begin;
            switch (step) {
                bcase HEADERS:
                    if (*(ptr) == ':') {
                        if (!__EXPECT_HEADER_VALUE__) {
                            __EXPECT_HEADER_VALUE__=1;
                            if (!strncmp(begin, get_header_name(CONTENT_LENGTH), len)) {
                                __IS_CONTENT_LENGTH__ = 1;
                            }
                            if (!strncmp(begin, get_header_name(HOST), len)) {
                                __IS_HOST__ = 1;
                            }
                            data->AddHeaderKey(begin, len);
                        } else if (__IS_HOST__) {
                            MALLOC_CPOY_STR(data->hostname, begin, len);
                        }
                        begin = ptr+1;
                    } else if (*(ptr) == '\r') {
                        __IS_NEWLINE__ = 1;
                        __EXPECT_HEADER_VALUE__ = 0;
                        if (*(ptr-1) == '\n') {
                            step=BODY;
                        } else {
                            while (*(begin) == ' ') {
                                --len;
                                ++begin;
                            }
                            if (__IS_CONTENT_LENGTH__) {
                                data->body_length = str2num(begin, len);
                                data->body = MALLOC_STR(data->body_length);
                                __IS_CONTENT_LENGTH__ = 0;
                            }
                            if (__IS_HOST__) {
                                if (data->hostname) {
                                    MALLOC_CPOY_STR(data->port, begin, len);
                                } else {
                                    MALLOC_CPOY_STR(data->hostname, begin, len);
                                    MALLOC_CPOY_STR(data->port, "80", 2);
                                }
                                __IS_HOST__ = 0;
                            }
                            data->AddHeaderValue(begin, len);
                        }
                        begin = ptr+1;
                    }
                bcase BODY:
                    body_offset = data->AppendBody(begin, body_offset, end-begin);
                    begin = end;
                    goto END_BODY;
            }
        }
        END_BODY:
        if (step == BODY && data->body_length == body_offset) {
            __EOF__ = 1;
        }
        return begin;
    }
};

template <>
size_t HTTP_Parser<HTTP_Request>::parse(CSTR at, int length);

template <>
size_t HTTP_Parser<HTTP_Response>::parse(CSTR at, int length);

template<>
HTTP_Parser<HTTP_Request>::HTTP_Parser(HTTP_Request *data);

template<>
HTTP_Parser<HTTP_Response>::HTTP_Parser(HTTP_Response *data);

#endif //BLOCK_CHAIN_HTTP_PARSER_H
