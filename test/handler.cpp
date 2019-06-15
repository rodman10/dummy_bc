//
// Created by huang on 19-6-8.
//

#include <json/json.h>
#include <block_chain/BlockChain.h>

extern huang::BlockChain<Transaction> block_chain;


route_entry(new_transaction) {
    Json::Value tx_data;
    auto flag = Json::Reader().parse(request.body, tx_data);
    if (!flag || tx_data["author"].isNull() || tx_data["content"].isNull()) {
        return HTTP_Response("Invlaid transaction data", HTTP_STATUS_NOT_ACCEPTABLE);
    }
    block_chain.AddNewTransaction(Transaction(tx_data["author"].asCString(), tx_data["content"].asCString(), time(NULL)));

    return HTTP_Response("Success", HTTP_STATUS_OK);
}

//route_entry(chain) {
//
//}
//
//route_entry(mine) {
//
//}
//
//route_entry(register_node) {
//
//}
//
//route_entry(register_with) {
//
//}
//
//route_entry(add_block) {
//
//}
//
//route_entry(pending_tx) {
//
//}

//TODO custom settings
bool InitRoute() {
    Register("/new_transaction", new_transaction, 1<<HTTP_POST);
}



