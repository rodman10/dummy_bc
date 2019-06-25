//
// Created by huang on 19-6-6.
//

#ifndef BLOCK_CHAIN_BLOCKCHAIN_H
#define BLOCK_CHAIN_BLOCKCHAIN_H

#include <util/precompiled.h>
#include <ctime>
#include <vector>
#include <string>
#include <functional>
#include <numeric>

#include <util/picosha2.h>
#include <unordered_set>
#include <http/request.h>
#include <json/json.h>
#include "Model.h"

#define all(v) (v).begin(),(v).end()


namespace huang {
    using namespace std;
    template<class T>
    struct Block {
        int nonce;
        int index;
        time_t timestamp;
        string previous_hash;
        string _hash;
        vector<T> transactions;
    public:
        Block(int index, time_t timestamp, const vector<T> &transactions, const string &previous_hash, int nonce = 0) : index(index),
                                                                                                                        timestamp(
                                                                                                                                timestamp),
                                                                                                                        transactions(
                                                                                                                                transactions),
                                                                                                                        previous_hash(
                                                                                                                                move(previous_hash)),
                                                                                                                        nonce(nonce) {
            _hash = this->ComputeHash();
        }

        Block(const Json::Value &data);

        Json::Value toJSON() const;

        string ComputeHash() const;
    };

    template<class T>
    struct BlockChain {
        vector<T> unconfirmed_transactions;
        vector<Block<T>> chain;
        int difficulty;
        string prefix;
    public:
        BlockChain(int difficulty=1) : difficulty(difficulty) {
            prefix = string(difficulty, '0');
        }

        BlockChain(const Json::Value &data);


        bool AddBlock(Block<T> block, string proof);

        bool isValidProof(const Block<T> &block, const string &block_hash);

        string ProofOfWork(Block<T> &block);

        void AddNewTransaction(const T &transaction) {
            unconfirmed_transactions.pb(transaction);
        }

        bool CheckChainValidity(const vector<Block<T>> &chain);

        int Mine();

        void CreateGenesisBlock() {
            chain.eb(0, time(nullptr), vector<T>(), "0");
        }
    };
}

bool Consensus();
void AnnounceNewBlock(const huang::Block<Transaction> &block);



namespace std {
    template <class T>
    struct hash<huang::Block<T>> {
        typedef huang::Block<T> argument_type;
        typedef string result_type;
        result_type operator()(argument_type const& s) const
        {
            auto res = to_string(s.index).
					append(to_string(s.nonce)).
                    append(to_string(s.timestamp)).
                    append(s.previous_hash);
            for (auto &&e : s.transactions) {
                string tmp = hash<T>{}(e);
                res.append(tmp);
            }
            return res;
        }
    };
}



template <typename T>
huang::Block<T>::Block(const Json::Value &data) {
    nonce = JSON2Type(data, nonce, Int);
    index = JSON2Type(data, index, Int);
    timestamp = JSON2Type(data, timestamp, Int64);
    previous_hash = JSON2Type(data, previous_hash, String);
    _hash = JSON2Type(data, _hash, String);
    JSON2VECTOR(data, transactions, [](const Json::Value &j) {
        return T(j);
    });
}

template <typename T>
Json::Value huang::Block<T>::toJSON() const {
    Json::Value data;
    DEFAULTTYPE2JSON(data, nonce);
    DEFAULTTYPE2JSON(data, index);
    TYPE2JSON(data, timestamp, int64_t);
    DEFAULTTYPE2JSON(data, previous_hash);
    DEFAULTTYPE2JSON(data, _hash);
    VECTOR2JSON(data, transactions, toJSON);
    return data;
}

template <typename T>
string huang::Block<T>::ComputeHash() const {
    string block_string(hash<Block<T>>{}(*this));
    return picosha2::hash256_hex_string(block_string);
}

template <typename T>
huang::BlockChain<T>::BlockChain(const Json::Value &data) {
    difficulty = JSON2Type(data, difficulty, Int);
    JSON2VECTOR(data, chain, [](const Json::Value &j) {
        return Block<T>(j);
    });
    JSON2VECTOR(data, unconfirmed_transactions, [](const Json::Value &j) {
        return T(j);
    });
}

template <typename T>
bool huang::BlockChain<T>::AddBlock(Block<T> block, string proof) {
    auto &previous_hash = chain.back()._hash;

    if (previous_hash != block.previous_hash || !isValidProof(block, proof)) {
        return 0;
    }

    block._hash = proof;
    chain.pb(block);
    return 1;
}

template <typename T>
bool huang::BlockChain<T>::isValidProof(const Block<T> &block, const string &block_hash) {
    return (block_hash.substr(0,difficulty) == prefix) && block_hash == block.ComputeHash();
}

template <typename T>
string huang::BlockChain<T>::ProofOfWork(Block<T> &block) {
    block.nonce = -1;
    string computed_hash;
    do {
        ++block.nonce;
        computed_hash = move(block.ComputeHash());
    } while (computed_hash.substr(0,difficulty) != prefix);
    return computed_hash;
}

template <typename T>
bool huang::BlockChain<T>::CheckChainValidity(const vector<Block<T>> &chain) {
    bool result = 1;
    string previous_hash = "0";

    for (auto &block : chain) {
        if (!isValidProof(block, block._hash) || previous_hash != block.previous_hash) {
            result = 0;
            break;
        }
        previous_hash = block._hash;
    }
    return result;
}

template <typename T>
int huang::BlockChain<T>::Mine() {
    if (unconfirmed_transactions.empty()) {
        return 0;
    }
    auto &last_block = chain.back();
    Block<Transaction> new_block(last_block.index+1, time(nullptr), unconfirmed_transactions, last_block._hash);
    auto proof = ProofOfWork(new_block);
    AddBlock(new_block, proof);

    unconfirmed_transactions.clear();
    AnnounceNewBlock(new_block);
    return new_block.index;
}

namespace std {
    template <>
    struct hash<huang::Block<string>> {
        typedef huang::Block<string> argument_type;
        typedef string result_type;
        result_type operator()(argument_type const& s) const
        {
            auto &res = to_string(s.index).
                    append(to_string(s.nonce)).
                    append(to_string(s.timestamp)).
                    append(s.previous_hash);
            for (auto &&e : s.transactions) {
                res.append(e);
            }
            return res;
        }
    };
}



#endif //BLOCK_CHAIN_BLOCKCHAIN_H
