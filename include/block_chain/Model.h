//
// Created by huang on 19-6-9.
//

#ifndef BLOCK_CHAIN_MODEL_H
#define BLOCK_CHAIN_MODEL_H

#include <util/precompiled.h>

struct Transaction {
    char *author, *content;
    time_t timestamp;

    Transaction(const char *author, const char *content, time_t timestamp) : timestamp(timestamp) {
        int n = strlen(author);
        this->author = malloc_str(n);
        copy_str(this->author, author, n);
        n = strlen(content);
        this->content = malloc_str(n);
        copy_str(this->content, content, n);
    }

    Transaction(const Json::Value &data) {
        JSON2CSTR(data, author, this->author);
        JSON2CSTR(data, content, this->content);
        timestamp = JSON2Type(data, timestamp, Int64);
    }

    Json::Value &&toJSON() const {
        Json::Value data;
        DEFAULTTYPE2JSON(data, author);
        DEFAULTTYPE2JSON(data, content);
        Type2JSON(data, timestamp, int64_t);
    }

    ~Transaction() {
        free(author);
        author = nullptr;
        free(content);
        content = nullptr;
    }
};


namespace std {
    template <>
    struct hash<Transaction> {
    typedef Transaction argument_type;
    typedef string result_type;
    result_type operator()(argument_type const& s) const
    {
        s;
        auto &res = string(s.author).
                append(s.content).
                append(to_string(s.timestamp));
        return res;
    }
};
}


#endif //BLOCK_CHAIN_MODEL_H
