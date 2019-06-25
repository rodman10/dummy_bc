//
// Created by huang on 19-6-15.
//

#include <stdio.h>
#include <util/precompiled.h>
#include <block_chain/BlockChain.h>

extern huang::BlockChain<Transaction> block_chain;
extern std::unordered_set<std::string> peers;

bool Consensus() {
    huang::BlockChain<Transaction> longest_chain;
    int current_len = block_chain.chain.size();
    bool flag = 0;
    for (auto &&node: peers) {
        STR url = MALLOC_STR(node.size() + 6);
        sprintf(url, "%s/chain", node.c_str());
        HTTP_Response response;
        if (HTTP_Get(url, response) == -1) {
            return 0;
        }
        Json::Value json;
        Json::Reader().parse(response.body, json);
        int length = json["length"].asInt();
        //TODO replace with chain struct
        huang::BlockChain<Transaction> chain(json["chain"]);
        if (length > current_len && block_chain.CheckChainValidity(chain.chain)) {
            current_len = length;
            longest_chain = std::move(chain);
            flag = 1;
        }

        if (flag) {
            block_chain = std::move(longest_chain);
            return 1;
        }
        return 0;
    }
}

void AnnounceNewBlock(const huang::Block<Transaction> &block) {
    for (auto &&node : peers) {
        STR url = MALLOC_STR(node.size() + 10);
        sprintf(url, "http://%s/add_block", node.c_str());
        HTTP_Response response;
        if (HTTP_Post(url, Json::FastWriter().write(block.toJSON()).c_str(), response) == -1) {
            puts("announce new block failed");
        }

    }
}