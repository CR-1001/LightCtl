/*
 * This file is part of LightCtl.
 * Copyright (C) 2022 Christian Rauch
 *
 * Distributed under terms of the GPL3 license.
 */

// Starting point. Conveys user input to the command interpreter and invokes the bridge connector.

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <map>
#include <bits/stdc++.h>
#include <list>
#include <algorithm>
#include <functional>
#include <ranges>

#include "bridge_connector.hpp"
#include "command_interpreter.hpp"
#include "output_formatter.hpp"
#include "state.hpp"

using namespace std;
using namespace boost;

static auto verbose = false;

int main(int argc, char *argv[], char *envp[]) {

  // TODO: better error handling
  try {

    // The bridge network address (required)
    string bridge = "";

    // The user (required)
    string user = "";

    // Will emit the JSON-serialized HTTP response JSON as received from the bridge
    auto raw = false;

    // Do not write table headers to output
    auto brief = false;

    // Use device names instead of device IDs
    auto alias = false;

    // Get the options, these are technical parameters 
    command_interpreter::parse_options(argc, argv, bridge, user, brief, verbose, raw, alias);

    // The connector interacts with the bridge REST API and writes the processed response to stdout
    auto connector = bridge_connector(bridge, user, raw, verbose);

    // Get the command (such as "set 7 on 320 80 50" or "state") without the options (such as --raw or --verbose)
    auto command = command_interpreter::extract_command(argc, argv);

    // Alias-to-ID mapper
    auto alias_mapper = function<uint(string&, bool&)>([&](string& id_or_alias, bool& group) {

      if(alias) {

        auto name_map = connector.get_name_map(group);

        id_or_alias = to_string(name_map[id_or_alias]);
      }

      return stoi(id_or_alias);
    });

    // Process the state command
    bool group = false;
    std::optional<vector<uint>> ids = nullopt;
    if (command_interpreter::parse_state_command(command, alias_mapper, ids, group)) {
      
      auto states = connector.get_states(ids, group);

      output_formatter::print_state(raw, brief, states.first, states.second);

      return 0;
    }

    // Process the set command
    auto state_to_set = state();
    if (command_interpreter::parse_set_command(command, alias_mapper, state_to_set)) {

      connector.set_state(state_to_set);

      auto ids = std::optional<vector<uint>> {vector<uint>{state_to_set.id}};
      auto state_after_set = connector.get_states(ids, state_to_set.group);

      output_formatter::print_state(raw, brief, state_after_set.first, state_after_set.second);

      return 0;
    }

    // Show help
    if (command_interpreter::parse_help_command(command)) {

      output_formatter::print_help();

      return 0;
    }
  }
  catch (std::exception ecception) {

    output_formatter::print_error();
  }

  output_formatter::print_info();

  return 1;
}