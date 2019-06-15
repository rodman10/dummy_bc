//
// Created by huang on 19-6-6.
//

#include <block_chain/BlockChain.h>

using namespace std;

bool TestBlockHash() {
    huang::Block<string> blk(1, time(NULL), vector<string>({"1rewr","2fd","3wq"}), "testhash");
    string res = blk.ComputeHash();
    return 1;
}

int main() {
    TestBlockHash();
}