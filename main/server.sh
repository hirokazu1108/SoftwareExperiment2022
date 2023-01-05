#!/bin/sh
#read -p "Input Client Num :"  clientNum
PASS='~/software/CreateGame2022'

gnome-terminal -- bash -c "echo 'sever'; cd ../server; ./SB_server $1 $2;bash"
