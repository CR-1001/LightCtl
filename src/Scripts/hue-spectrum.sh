#!/bin/bash

# runs the hue spectrum (0..360) for a single device (first argument).

# set bridge and user here
lightctl="../lightctl --bridge=192.168.0.206 --user=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX --alias --brief "

# hue from 0 to 360 in steps of 10
hue=0
maxHue=360
incrementHue=10
while [ $hue -le $maxHue ]
do
  $lightctl set $1 on $hue 100 100
  ((hue=$hue+$incrementHue))
  sleep 1s
done
