#!/bin/bash -i

# simple integration test to check the commands with real devices

# set bridge and user here
lightctl_test="../lightctl --bridge=192.168.0.206 --user=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX "

clear

# evaluates a single command
test() {

    echo "---- TEST ----"
    echo "lightctl" $*
    $lightctl_test $*
    sleep 4s
    echo
}

# actual tests
test "state"

test "state Couch --alias"
test "state 4"

test "state-group"

test "state-group Küche --alias"
test "state-group 2"

test "set 4 on"
test "set 4 on 70"
test "set 4 on green 30"
test "set Couch on 40 50 70 --alias"
test "set Couch on 60  -  - --alias"
test "set Couch on  - 70  - --alias"
test "set Couch on  -  - 90 --alias"
test "set 4 off"

test "set-group 2 on blue 70"
test "set-group Wohnzimmer on 40 50 80 --alias"
test "set-group 2 off"

test "state-group"
test "state"
