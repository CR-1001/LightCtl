#!/bin/bash

# set bridge and user here
lightctlx="../lightctl --bridge=192.168.0.206 --user=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " 

# get all device names
ids=$($lightctlx state --brief | awk '{print $6}')

# iterate over them
for id in $ids
do
    # turn the device on for a second
    ./set-and-undo.sh 1s $id on
done
