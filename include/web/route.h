//
// Created by huang on 19-6-8.
//

#ifndef BLOCK_CHAIN_ROUTE_H
#define BLOCK_CHAIN_ROUTE_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <http/http.h>

#define NODE_NUM 128
#define route_entry(func) HTTP_Response func(const HTTP_Request &request)


typedef HTTP_Response (*ROUTE_CB)(const HTTP_Request &);

struct trie_node {
    trie_node *nodes[NODE_NUM];
    char val;
    ROUTE_CB cb;
    int methods;
    trie_node(char val) : val(val) {}

    ~trie_node() {
        for (auto &node : nodes) {
            delete node;
        }
    }
};

struct trie_tree {
    trie_node *root;

    trie_tree() {
        root = new trie_node(0);
    }

    virtual ~trie_tree() {
        delete root;
    }

    bool Insert(const char *s, ROUTE_CB cb, int methods) {
        int n = strlen(s);
        if (!n) {
            return 0;
        }
        auto rt = root;
        for (int i = 0; i < n; ++i) {
            auto &&c = s[i];
            if (!rt->nodes[c]) {
                rt->nodes[c] = new trie_node(c);
            }
            rt = rt->nodes[c];
        }
        rt->cb = cb;
        rt->methods = methods;
        return 1;
    }
    
    ROUTE_CB Find(const char *s, int method) {
        int n= strlen(s);
        if (!n) {
            return nullptr;
        }
        auto rt = root;
        for (int i = 0; i < n; ++i) {
            auto &&c = s[i];
            if (!rt->nodes[c])return 0;
            rt = rt->nodes[c];
        }
        //TODO add method judge
        if (!rt->cb || !(rt->methods & method)) {
            return nullptr;
        }
        return rt->cb;
    }
};

static trie_tree tree;

bool Register(const char *path, ROUTE_CB cb, int methods);

ROUTE_CB Route(const char *path, int method);

#endif //BLOCK_CHAIN_ROUTE_H
