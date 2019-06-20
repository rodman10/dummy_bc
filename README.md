# Dummy Block Chain

## Structure

### include

- block_chain: block chain code

- http: httpd code

- util: just some common code

- web: simple route implemented by trie

### src

the implement code

### test

- handler.cpp: route handler functions

- httpd.cpp: the server

## Dependency

- http_parser

- pthread

- jsoncpp

## API

- 添加transactions

    |属性|值|
    |:-----|:-----|
    |url|/new_transaction|
    |method|POST|
    |Content-Type|application/json|

    请求数据格式

    ```json
    {
        "file_name": "file_name", 
        "file_size": 163, 
        "file_hash": "hash", 
        "file_origin_hash": "origin_hash"
    }
    ```

    返回数据

    |属性|值|
    |:-----|:-----|
    |状态码|200/406|
    |Content-Type|text/plain|

    - 406: Invalid transaction data
    - 200: Success

- 添加block

    |属性|值|
    |:-----|:-----|
    |url|/mine|
    |method|GET|

    返回数据

    |属性|值|
    |:-----|:-----|
    |状态码|200|
    |Content-Type|text/plain|

    - 200:
        - No transactions to mine.
        - Block %d is mined.

- 读取chain

    |属性|值|
    |:-----|:-----|
    |url|/chain|
    |method|GET|

    返回数据

    |属性|值|
    |:-----|:-----|
    |状态码|200|
    |Content-Type|text/plain,application/json|
    
    - 200:
        - No transactions to mine.
        - 
        ```json
        {
            "chain": [
                {
                    "_hash": "7a25e9ea63e293ba36183af66a9e623c242c06c49fce2f5f1360f4370be67ba0",
                    "index": 0,
                    "nonce": 0,
                    "previous_hash": "0",
                    "timestamp": 1561010400,
                    "transactions": []
                },
                {
                    "_hash": "060e01e92f93f34f92a51f7007c023ca7f3984c64e6d4db889406ebc42940ba2",
                    "index": 1,
                    "nonce": 2,
                    "previous_hash": "7a25e9ea63e293ba36183af66a9e623c242c06c49fce2f5f1360f4370be67ba0",
                    "timestamp": 1561010424,
                    "transactions": [
                        {
                            "file_hash": "hash",
                            "file_name": "test3",
                            "file_origin_hash": "ddsafs",
                            "file_size": 163,
                            "timestamp": 1561010411
                        }
                    ]
                }
            ],
            "length": 2,
            "peers": []
        }
        ```