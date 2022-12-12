/*
 * This file is part of LightCtl.
 * Copyright (C) 2022 Christian Rauch
 *
 * Distributed under terms of the GPL3 license.
 */

// Maps user input to commands and parameters.

#include <string>
#include <string_view>
#include <boost/tokenizer.hpp>
#include <optional>

#include "command_interpreter.hpp"

using namespace std;
using namespace boost;

// Extracts the commands (such as "set 7 on 320 80 50" or "state") and ignores the options
vector<string> command_interpreter::extract_command(int argc, char *argv[]) {

  auto command = vector<string>();
  int i = 1;

  while(i < argc) {
    
    auto arg = string(argv[i++]);

    if(!arg.starts_with("--")) {
      command.push_back(arg);
    }
  }

  return command;
}

// Returns true if it is a help command
bool command_interpreter::parse_help_command(vector<string>& command) {

  return command.size() >= 1 && command[0] == "help";
}

// Returns true if it is a set command and fills the parameters
bool command_interpreter::parse_set_command(
  vector<string>& command,
  function<uint(string&, bool&)>& name_map, 
  state& state) {

  if(command.size() < 3 
    || (command[0] != "set" && command[0] != "set-group") 
    || command[1] == "") {

    return false;
  }

  state.group = command[0] == "set-group";

  state.id = name_map(command[1], state.group);

  state.on = command[2] == "on";

  auto const not_set = "-";

  string hue_str = not_set;
  string sat_str = not_set;
  string bri_str = not_set;

  bool match = false;

  // Find the proper command version by word count
  switch (command.size()) {

    // set <id> <on/off>
    case 3:
      match = true;
      break;

    // set <id> <on/off> <brightness: 0..100>"
    case 4:
      bri_str = command[3];
      match = true;
      break;

    // set <id> <on/off> <hue: 0..360> <brightness: 0..100>
    case 5:
      hue_str = command[3];
      sat_str = "100";
      bri_str = command[4];
      match = true;
      break;

    // set <id> <on/off> <hue: 0..360> <saturation: 0..100> <brightness: 0..100>
    case 6:
      hue_str = command[3];
      sat_str = command[4];
      bri_str = command[5];
      match = true;
      break;
  }

  if (hue_str != not_set) state.set_hue360(stoi(get_hue360(hue_str)));
  if (sat_str != not_set) state.set_sat100(stoi(sat_str));
  if (bri_str != not_set) state.set_bri100(stoi(bri_str));

  return match;
}

// Returns true if it is a state command and fills the parameters
bool command_interpreter::parse_state_command(
  vector<string>& command,
  function<uint(string&, bool&)>& name_map,
  std::optional<vector<uint>>& ids,
  bool& group) {

  if(command.size() < 1 || (command[0] != "state" && command[0] != "state-group")) {

    return false;
  }

  group = command[0] == "state-group";

  if(command.size() == 2) {

    auto id = name_map(command[1], group);
    ids = std::optional(vector<uint>{id});
  }
  else {
    ids = std::nullopt;
  }

  return true;
}

// Sets the options, these are technical parameters, 
// either bridge communication or output related
void command_interpreter::parse_options(
  int argc,
  char *argv[],
  string& bridge,
  string& user,
  bool& brief,
  bool& verbose,
  bool& raw,
  bool& alias) {
  
  int i = 0;
  while(i < argc) {
    
    auto arg = string(argv[i++]);

    if (arg.starts_with("--verbose")) {
      verbose = true;
    }
    else if (arg.starts_with("--raw")) {
      raw = true;
    }
    else if (arg.starts_with("--alias")) {
      alias = true;
    }
    else if (arg.starts_with("--brief")) {
      brief = true;
    }
    else if (arg.starts_with("--bridge")) {
      // --bridge=abcdefgh
      // 0123456789
      bridge = arg.substr(9);
    }
    else if (arg.starts_with("--user")) {
      // --user=abcdefgh
      // 0123456789
      user = arg.substr(7);
    }
  }
}

// Converts a color name to a hue degree
string command_interpreter::get_hue360(string value) {

  if(value == "red") return "0";
  if(value == "orange") return "30";
  if(value == "yellow") return "60";
  if(value == "lime") return "90";
  if(value == "green") return "120";
  if(value == "turquoise") return "150";
  if(value == "cyan") return "180";
  if(value == "azure") return "210";
  if(value == "blue") return "240";
  if(value == "violet") return "270";
  if(value == "magenta") return "300";
  if(value == "rose") return "330";

  return value;
}