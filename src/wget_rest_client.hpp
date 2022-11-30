/*
 * This file is part of LightCtl.
 * Copyright (C) 2022 Christian Rauch
 *
 * Distributed under terms of the GPL3 license.
 */

// Communication with REST APIs. To be replaced with a cURL based implementation.

#ifndef __WGETRESTCLIENT_H__
#define __WGETRESTCLIENT_H__

#include <bits/stdc++.h>
#include <boost/format.hpp>
#include <atomic>
#include <sstream>
#include <unistd.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <regex.h>
#include <string>

#define LOG_VERBOSE(msg) \
  if(this->verbose) { \
    cout \
      << "[" << __func__ << "] " << endl \
      << format msg << endl << endl; \
  }

using namespace std;
using namespace boost;

class wget_rest_client {

  public:

    bool verbose = false;

    wget_rest_client(bool verbose);

    // Checks if commands can be invoked
    void check_preconditions();

    // Calls a web url using wget
    string call_web_url(string http_verb, string url, string& body);

    // Executes a system command
    string execute_command(string& command);

    // Converts JSON to a property tree
    boost::property_tree::ptree convert_json_to_property_tree(string& json);
};

#endif