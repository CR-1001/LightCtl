/*
 * This file is part of LightCtl.
 * Copyright (C) 2022 Christian Rauch
 *
 * Distributed under terms of the GPL3 license.
 */

// Prints the device infos and the help.

#include <iostream>
#include <algorithm>
#include <string>
#include <bits/stdc++.h>
#include <map>
#include <boost/format.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "output_formatter.hpp"
#include "state.hpp"

using namespace std;
using namespace boost;

// Format value
template<typename t> 
string output_formatter::format_fallback(std::optional<t>& value, const string& fallback) {

  if(value.has_value()) {

    return (format{"%3.0f"} % value.value()).str();
  } 

  return fallback;
}

// Prints the device states
void output_formatter::print_state(bool raw, bool brief, vector<state>& states, string& json) {

  if (raw) {

    cout << json << endl;

    return;
  }

  if (!brief) {
    cout << "  ID PWR HUE SAT BRI NAME" << endl;
  }

  const string not_present = "-";
  for (auto &state : states) {

    auto hue = state.get_hue360();
    auto sat = state.get_sat100();
    auto bri = state.get_bri100();

    cout << format{"%4.0f %d %3.0f %3.0f %3.0f %d"}
      % state.id 
      % (state.on ? " on" : "off") 
      % (format_fallback<uint>(hue, not_present))
      % (format_fallback<ushort>(sat, not_present))
      % (format_fallback<ushort>(bri, not_present))
      % state.name 
      << endl;
  }
}

// Prints the info
void output_formatter::print_info() {

  cout
    << endl
    << "LightCtl allows you to control your Philips Hue lights." << endl
    << endl
    << "Commands to query and set device states:" << endl
    << "  lightctl state[-group] [<id>]" << endl
    << "  lightctl set[-group] <id> <on/off> <hue: 0..360> [<saturation: 0..100>] <brightness: 0..100>" << endl
    << endl
    << "Use the help for a complete command list:" << endl
    << "  lightctl help" << endl
    << endl;
}

// Prints the error message
void output_formatter::print_error() {

  cerr
    << "Error occured." << endl
    << "Use option --verbose to see execution details." << endl
    << "Verify that you have specified the proper bridge address and user." << endl
    << endl;
}

// Prints the help
void output_formatter::print_help() {

  cout
    << "# LightCtl 0.8" << endl
    << endl
    << "LightCtl allows you to control your Philips Hue lights (API version 1)." << endl
    << "You can query device states or set device attributes (such as brightness or hue)." << endl
    << endl
    << "Query device or group states with:" << endl
    << "  lightctl state[-group] [<id>]" << endl
    << endl
    << "This will print a device or group table for all or the specified IDs with following columns:" << endl
    << "  ID    Device or group ID" << endl
    << "  PWR   On or off    on/off" << endl
    << "  HUE   Hue value    0..360 or -" << endl
    << "  SAT   Saturation   0..100 or -" << endl
    << "  BRI   Brightness   0..100 or -" << endl
    << "  NAME  Device or group name" << endl
    << endl
    << "Set device or group parameters (on/off, hue, saturation, and brightness) with:" << endl
    << "  lightctl set[-group] <id> <on/off> [<brightness: 0..100 or>]" << endl
    << "  lightctl set[-group] <id> <on/off> <hue: 0..360> [<saturation: 0..100>] <brightness: 0..100>" << endl
    << "  " << endl
    << "This will also print the device or group table for the specified device or group." << endl
    << endl
    << "Color names can be used instead of hue values:" << endl
    << "  red, orange, yellow, lime, green, turquoise, cyan, azure, blue, violet, magenta, rose." << endl
    << "  " << endl
    << "No value (-) is printed for HUE and SAT if the device does not support colors." << endl
    << "In a set command, a value can be omitted (-) to preserve the attribute's current value." << endl
    << endl
    << "Options:" << endl
    << "  --bridge=<...>  Set the bridge network address                     (required)" << endl
    << "  --user=<...>    Set the user                                       (required)" << endl
    << "  --alias         Use device names instead of IDs for <id>" << endl
    << "  --raw           Emit raw JSON output (HTTP response from the bridge REST API)" << endl
    << "  --verbose       Enable verbose output" << endl
    << "  --brief         Disable table header line" << endl
    << endl
    << "Example commands:" << endl
    << "  lightctl state Desk               --alias --bridge=<...> --user=<...>" << endl
    << "  lightctl state-group                      --bridge=<...> --user=<...>" << endl
    << "  lightctl state-group 3                    --bridge=<...> --user=<...>" << endl
    << "  lightctl state-group Kitchen      --alias --bridge=<...> --user=<...>" << endl
    << "  lightctl set 7 on 320 80 50               --bridge=<...> --user=<...>" << endl
    << "  lightctl set 8 on 120 - -                 --bridge=<...> --user=<...>" << endl
    << "  lightctl set 11 on orange 100             --bridge=<...> --user=<...>" << endl
    << "  lightctl set Couch on - 80        --alias --bridge=<...> --user=<...>" << endl
    << "  lightctl set-group Kitchen on 100 --alias --bridge=<...> --user=<...>" << endl
    << endl
    << "Example output:" << endl
    << "  ID PWR HUE SAT BRI NAME" << endl
    << "   1  on  40  90  50 Couch" << endl
    << "   2 off   -   -  50 Living room" << endl
    << "   3 off 240  60 100 Kitchen" << endl
    << "   4  on  30  60 100 Kitchen 2" << endl
    << endl
    << "Remarks:" << endl
    << "  - The program requires wget." << endl
    << "  - You might define a shell alias that incorporates --bridge=<...> and --user=<...>." << endl
    << "  - Please refer to https://developers.meethue.com for creating an user account on the bridge. " << endl
    << "    The process usually involves calling a specific RESTful API (such as <bridge address>" << endl
    << "    /api/newdeveloper) while pushing a physical button on the bridge. " << endl
    << "  - A program like nmap can be helpful to find the bridge network address." << endl
    << "  - Group states seem to report the state of one of the devices in the group. " << endl
    << endl
    << "Disclaimer:" << endl
    << "  This program is free software; you can redistribute it and/or modify it under" << endl
    << "  the terms of the GNU General Public License (GPL) version 3 as published by " << endl
    << "  the Free Software Foundation version." << endl
    << "  This program is distributed in the hope that it will be useful, but without" << endl
    << "  any warranty; without even the implied warranty of merchantability or fitness" << endl
    << "  for a particular purpose. " << endl
    << endl
    << "Author:" << endl
    << "  Christian Rauch" << endl
    << endl;
}