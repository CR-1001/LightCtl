/*
 * This file is part of LightCtl.
 * Copyright (C) 2022 Christian Rauch
 *
 * Distributed under terms of the GPL3 license.
 */

// Communicates with the Philips Hue bridge, queries device states and sets device attributes.
// Requires the bridge address and the bridge user.

#ifndef __BRIDGECONNECTOR_H__
#define __BRIDGECONNECTOR_H__

#include <sstream>
#include <unistd.h>
#include <map>
#include <boost/property_tree/json_parser.hpp>
#include <boost/format.hpp>

#include "state.hpp"

using namespace std;

class bridge_connector {

  public:

    string bridge;
    string user;
    bool raw;
    bool verbose;

    bridge_connector(string& bridge, string& user, bool raw, bool verbose);

    // Sets device attributes
    void set_state(state& state);

    // Gets device states (on/off, hue, ...)
    pair<vector<state>,string> get_states(std::optional<vector<uint>>& device_ids, bool group);

    // Maps a device name to a device ID
    map<string,int> get_name_map(bool group);

    // Gets the bridge api URL
    string get_bridge_address();
};

#endif