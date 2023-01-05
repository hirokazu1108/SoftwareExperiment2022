#!/bin/sh
#read -p "Input Client Num :"  clientNum
PASS='~/software/CreateGame2022'

if [ $1 = "l" ]; then
gnome-terminal -- bash -c "echo 'client'; cd ../client; ./SB_client localhost $2; bash"
else
gnome-terminal -- bash -c "echo 'client'; cd ../client; ./SB_client clpc$1 $2; bash"
fi