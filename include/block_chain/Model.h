//
// Created by huang on 19-6-9.
//

#ifndef BLOCK_CHAIN_MODEL_H
#define BLOCK_CHAIN_MODEL_H

#include <util/precompiled.h>
using std::string;

struct Transaction {
    string author, content;
    time_t timestamp;

    Transaction(const char *author, const char *content, time_t timestamp) : timestamp(timestamp), author(author), content(content) {
    }

    Transaction(const Json::Value &data) {
        author = std::move(data["author"].asString());
        content = std::move(data["content"].asString());
        timestamp = JSON2Type(data, timestamp, Int64);
    }

    Json::Value &&toJSON() const {
        Json::Value data;
        DEFAULTTYPE2JSON(data, author);
        DEFAULTTYPE2JSON(data, content);
        Type2JSON(data, timestamp, int64_t);
        return std::move(data);
    }

    ~Transaction() {
    }
};


namespace std {
    template <>
    struct hash<Transaction> {
    typedef Transaction argument_type;
    typedef string result_type;
    result_type operator()(argument_type const& s) const
    {
        return string(s.author).
                append(s.content).
                append(to_string(s.timestamp));
    }
};
}


#endif //BLOCK_CHAIN_MODEL_H
