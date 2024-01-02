/*
 * This file is part of LightCtl.
 * Copyright (C) 2022 Christian Rauch
 *
 * Distributed under terms of the GPL3 license.
 */

// Communicates with the Philips Hue bridge, queries device states and sets device attributes.
// Requires the bridge address and the bridge user.

#include <sstream>
#include <unistd.h>
#include <map>
#include <boost/format.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "wget_rest_client.hpp"
#include "bridge_connector.hpp"

using namespace std;

bridge_connector::bridge_connector(string& bridge, string& user, bool raw, bool verbose) {
  this->bridge = bridge;
  this->user = user;
  this->raw = raw;
  this->verbose = verbose;

  LOG_VERBOSE({ "Philips Hue Connect initialized (process ID %1%)" } % getpid())
  LOG_VERBOSE({ "Bridge address '%1%' and user ID '%2%'" } % bridge % user)
}

// Sets device attributes
void bridge_connector::set_state(state& state) {

  auto json = state.to_json();

  LOG_VERBOSE({"Setting '%1%' for device '%2%'"} % json % state.id)

  string base_url = state.group ? "%1%/groups/%2%/action" : "%1%/lights/%2%/state";

  string url = (boost::format{base_url} 
        % get_bridge_address() 
        % state.id)
      .str();

  LOG_VERBOSE({"Calling '%1%' "} % url)

  auto restclient = wget_rest_client(verbose);

  auto result = restclient.call_web_url("PUT", url, json);

  LOG_VERBOSE({"Response:\n%1%"} % result)
}

// Gets device states (on/off, hue, ...)
pair<vector<state>,string> bridge_connector::get_states(std::optional<vector<uint>>& device_ids, bool group) {

  auto restclient = wget_rest_client(verbose);

  string base_url = group ? "%1%/groups" : "%1%/lights";

  // invoking the RESTful API
  string body = "";
  auto json = restclient.call_web_url("GET", 
    (boost::format{base_url} % get_bridge_address()).str(),
     body);

  // converting the JSON result to a processable property tree
  auto ptree = restclient.convert_json_to_property_tree(json);

  auto states = vector<state>();

  // we are loading the states for all devices and are filtering those that were not queried
  auto filter = device_ids.has_value();

  // create states for the devices listed in the property tree
  for (auto &device : ptree) {

    auto id = stoi(device.first);

    if (filter 
        && find(
          device_ids.value().begin(), 
          device_ids.value().end(), 
          id) == device_ids.value().end()) continue;

    auto s = state();
  
    s.id = id;
    s.group = group;

    auto attributes = device.second.find(group ? "action" : "state")->second;

    s.name     = device.second.get<string>("name");
    s.on       = attributes.get<bool>("on");

    s.hue65535 = attributes.get<int>("hue", 65536);
    s.sat254   = attributes.get<int>("sat", 255);
    s.bri254   = attributes.get<int>("bri", 255);

    if (group) {
      auto members = device.second.get_child("lights");
      for (auto m : members) {
        s.members.push_back(stoi(m.second.data()));
      }
    }

    states.push_back(s);
  }

  return make_pair(states, json);
}

// Maps a device name to a device ID
map<string,int> bridge_connector::get_name_map(bool group) {

  // the ID to name mapping is extracted from the device states
  std::optional<vector<uint>> device_ids = std::nullopt;
  auto states = get_states(device_ids, group).first;

  auto name_map = map<string,int> ();

  for(auto s : states) name_map[s.name] = s.id;

  return name_map;
}

// Gets the bridge api URL
string bridge_connector::get_bridge_address() {

  return (boost::format {"%1%/api/%2%"} % bridge % user).str();
}

