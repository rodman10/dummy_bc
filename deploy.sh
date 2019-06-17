#!/bin/bash

libs=("http-parser" "jsoncpp")
urls=("https://github.com/nodejs/http-parser/archive/master.zip" "https://github.com/open-source-parsers/jsoncpp/archive/master.zip")

for ((i=0; i<2; i++))
do
    wget urls[i] -O $libs[i].zip
    unzip $libs[i].zip
    rm $libs[i].zip
    cd "$libs[i]-master"
    make
    sudo make install
    cd ..
    rm -rf "$libs[i]-master"
done
sudo ldconfig