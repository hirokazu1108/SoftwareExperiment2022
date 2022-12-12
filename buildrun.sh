#!/bin/sh
#read -p "Input Client Num :"  clientNum
PASS='~/software/CreateGame2022'
gnome-terminal -- bash -c "echo 'sever'; cd $PASS/server; ./SB_server $1; bash; "

for i in `seq 1 $1`
do
gnome-terminal -- bash -c "echo 'client'; cd $PASS/client; echo a | ./SB_client; bash"
done
