/*
 * This file is part of LightCtl.
 * Copyright (C) 2022 Christian Rauch
 *
 * Distributed under terms of the GPL3 license.
 */

// Maps user input to commands and parameters.

#ifndef __COMMANDINTERPRETER_H__
#define __COMMANDINTERPRETER_H__

#include <string>
#include <string_view>
#include <functional>
#include <optional>
#include <boost/tokenizer.hpp>

#include "state.hpp"

using namespace std;
using namespace boost;

class command_interpreter{

  public:

    // Extracts the commands (such as "set 7 on 320 80 50" or "state") and ignores the options
    static vector<string> extract_command(int argc, char *argv[]);

    static bool parse_help_command(vector<string>& command);

    // Returns true if it is a set command and fills the parameters
    static bool parse_set_command(
      vector<string>& command,
      std::function<uint(string&, bool&)>& name_map, 
      state& state);

    // Returns true if it is a state command and fills the parameters
    static bool parse_state_command(
      vector<string>& command,
      std::function<uint(string&, bool&)>& name_map,
      std::optional<vector<uint>>& ids,
      bool& group);

    // Sets the options, these are technical parameters, 
    // either bridge communication or output related
    static void parse_options(
      int argc,
      char *argv[],
      string& bridge,
      string& user,
      bool& brief,
      bool& verbose,
      bool& raw,
      bool& alias);

    // Converts a color name to a hue degree
    static string get_hue360(string value);
};

#endif