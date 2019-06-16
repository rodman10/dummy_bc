//
// Created by huang on 19-6-9.
//

#include <http/request.h>

int main() {
//    int n = strlen(NULL);
    STR response = HTTP_Get("http://www.baidu.com/");
    puts(response);
}