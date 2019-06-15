//
// Created by huang on 19-6-9.
//

#include <web/route.h>

bool Register(const char *path, ROUTE_CB cb, int methods) {
    if (!path || !cb) {
        return 0;
    }
    return tree.Insert(path, cb, methods);
}

ROUTE_CB Route(const char *path, int method) {
    if(!path) {
        return nullptr;
    }
    return tree.Find(path, method);
}