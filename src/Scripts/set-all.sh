#!/bin/bash

# Set the same state for all devices
echo "lightctl set <all> $@"

# set bridge and user here
lightctlx="../lightctl --bridge=192.168.0.206 --user=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " 

# get all device IDs
ids=$($lightctlx state --brief | awk '{print $1}')

# iterate over them
for id in $ids
do
    # turn the device off
    $lightctlx set $id $@ > /dev/null
done

# display state after change
$lightctlx state