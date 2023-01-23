#!/bin/sh
#read -p "Input Client Num :"  clientNum
PASS='~/software/CreateGame2022'

cd ../client
make clean
make
cd ../server
make clean
make
cd ../main
make clean
make
