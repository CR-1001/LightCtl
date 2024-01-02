#!/bin/bash

# Sets a value for a device and undoes the change after a specified time span.

# set bridge and user here
lightctlx="../lightctl --bridge=192.168.0.206 --user=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX --alias "

# reset time
reset_after=$1

# device name
device=$2

# new state
tmp_pwr=$3
tmp_hue=$4
tmp_sat=${5:-100}
tmp_bri=${6:-100}
tmp_state="$tmp_pwr $tmp_hue $tmp_sat $tmp_bri"

# get current state
org_state=$($lightctlx state $device --brief | awk '{st=$2 " " $3 " " $4 " " $5; print st }')

# display current state
$lightctlx state $device

# change state
echo "┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄ temporary state ($reset_after)"
$lightctlx set $device $tmp_state --brief

# message
echo "┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄"

# sleep
sleep $reset_after

# change state
$lightctlx set $device $org_state --brief