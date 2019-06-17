//
// Created by huang on 19-6-9.
//

#ifndef BLOCK_CHAIN_MODEL_H
#define BLOCK_CHAIN_MODEL_H

#include <util/precompiled.h>
using std::string;

struct Transaction {
    string file_name, file_hash, file_origin_hash;
    long file_size;
    time_t timestamp;

    Transaction(const string &file_name, const string &file_hash, const string &file_origin_hash, long file_size,
                time_t timestamp) : file_name(file_name), file_hash(file_hash), file_origin_hash(file_origin_hash),
                                    file_size(file_size), timestamp(timestamp) {}

    Transaction(const Json::Value &data) {
        file_name = JSON2Type(data, file_name, String);
        file_hash = JSON2Type(data, file_hash, String);
        file_origin_hash = JSON2Type(data, file_origin_hash, String);
        file_size = JSON2Type(data, file_size, Int64);
        timestamp = JSON2Type(data, timestamp, Int64);
    }

    Json::Value toJSON() const {
        Json::Value data;
        DEFAULTTYPE2JSON(data, file_name);
        DEFAULTTYPE2JSON(data, file_hash);
        DEFAULTTYPE2JSON(data, file_origin_hash);
        DEFAULTTYPE2JSON(data, file_size);
        TYPE2JSON(data, timestamp, int64_t);
        return data;
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
        return string(s.file_name).
                append(s.file_hash).
                append(s.file_origin_hash).
                append(to_string(s.file_size)).
                append(to_string(s.timestamp));
    }
};
}


#endif //BLOCK_CHAIN_MODEL_H
