//
// Created by huang on 19-6-8.
//

#include <vector>
#include <json/json.h>
#include <block_chain/BlockChain.h>

using std::vector;
using huang::Block;

extern huang::BlockChain<Transaction> block_chain;
extern std::unordered_set<std::string> peers;

route_entry(new_transaction) {
    Json::Value tx_data;
    auto flag = Json::Reader().parse(request.body, tx_data);
    if (!flag || tx_data["file_name"].isNull() || tx_data["file_hash"].isNull() ||
            tx_data["file_origin_hash"].isNull() || tx_data["file_size"].isNull()) {
        return {"Invalid transaction data", TEXT_PLAIN, HTTP_STATUS_NOT_ACCEPTABLE};
    }
    block_chain.AddNewTransaction(Transaction(tx_data["file_name"].asString(),
                                              tx_data["file_hash"].asString(),
                                              tx_data["file_origin_hash"].asString(),
                                              tx_data["file_size"].asInt64(),
                                              time(nullptr)));

    return {"Success", TEXT_PLAIN, HTTP_STATUS_OK};
}

route_entry(chain) {
//    Consensus();
    Json::Value data;
    auto length = block_chain.chain.size();
    DEFAULTTYPE2JSON(data, length);
    auto &chain = block_chain.chain;
    VECTOR2JSON(data, chain, toJSON)
    DEFAULTVECTOR2JSON(data, peers);
    auto &&res = Json::FastWriter().write(data);
    return {res.c_str(), APPLICATION_JSON, HTTP_STATUS_OK};
}

route_entry(mine) {
    auto res = block_chain.Mine();
    if (!res) {
        return {"No transactions to mine", TEXT_PLAIN, HTTP_STATUS_OK};
    }
    // const str length is 16
    auto buf = MALLOC_STR(num_len(res) + 16);
    sprintf(buf, "Block %d is mined.", res);
    HTTP_Response response = {buf, TEXT_PLAIN, HTTP_STATUS_OK};
    FREE(buf);
    return std::move(response);
}

route_entry(register_node) {
    auto node_address = request.body;
    if (!node_address) {
        return {"Invalid data", TEXT_PLAIN, HTTP_STATUS_BAD_REQUEST};
    }
    peers.emplace(node_address);
    return chain(request);
}

route_entry(register_with) {
    auto node_address = request.body;
    if (!node_address) {
        return {"Invalid data", TEXT_PLAIN, HTTP_STATUS_BAD_REQUEST};
    }
    Json::Value data;
    GET_HEADER(request, HOST, host);
    data["node_address"] = host;
    auto buf = MALLOC_STR(strlen(host)+14);
    sprintf(buf, "%s/register_node", data.asCString());

    HTTP_Headers headers;
    ADD_HEADER_WITH_OBJ(headers, CONTENT_TYPE, APPLICATION_JSON);
    HTTP_Response response;
    HTTP_Post(buf, headers, Json::FastWriter().write(data).c_str(), response);
    FREE(buf);
    //TODO add http error
    Json::Value res;
    Json::Reader().parse(response.body, res);
    block_chain = huang::BlockChain<Transaction>(res["chain"]);
    JSON2SET(res["peers"], peers, asString);
    return {"Registration successful", APPLICATION_JSON, HTTP_STATUS_OK};
}

route_entry(add_block) {
    Json::Value block_data;
    Json::Reader().parse(request.body, block_data);
    Block<Transaction> block(block_data);
    auto proof = block_data["_hash"].asCString();
    auto added = block_chain.AddBlock(block, proof);
    if (!added) {
        return {"The block was discarded by the node", TEXT_PLAIN, HTTP_STATUS_BAD_REQUEST};
    }
    return {"Block added to the chain", TEXT_PLAIN, HTTP_STATUS_CREATED};
}

route_entry(pending_tx) {
    Json::Value data;
    auto& unconfirmed_transactions = block_chain.unconfirmed_transactions;
    VECTOR2JSON(data, unconfirmed_transactions, toJSON);
    return {data.asCString(), APPLICATION_JSON, HTTP_STATUS_OK};
}

//TODO custom settings
bool InitRoute() {
    Register("/new_transaction", new_transaction, 1<<HTTP_POST);
    Register("/chain", chain, 1<<HTTP_GET);
    Register("/mine", mine, 1<<HTTP_GET);
}



