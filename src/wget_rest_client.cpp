/*
 * This file is part of LightCtl.
 * Copyright (C) 2022 Christian Rauch
 *
 * Distributed under terms of the GPL3 license.
 */

// Communication with REST APIs. To be replaced with a cURL based implementation.

#include <bits/stdc++.h>
#include <boost/format.hpp>
#include <atomic>
#include <sstream>
#include <unistd.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <regex.h>
#include <string>

#include "wget_rest_client.hpp"

using namespace std;

wget_rest_client::wget_rest_client(bool verbose) {

  this->verbose = verbose;
}

// Checks if commands can be invoked
void wget_rest_client::check_preconditions() {

  if (!system(NULL)) {

    throw std::exception();
  }
}

// Calls a web url using wget
string wget_rest_client::call_web_url(string http_verb, string url, string& body) {

  auto body_data 
    = (body.empty() ? "" 
    : (boost::format { "--body-data='%1%' " } % body).str());

  auto command 
    = (boost::format {"wget %3%--method=%2% %4%-O - %1%"} 
      % url
      % http_verb 
      % body_data
      % (verbose ? "" : "-q "))
    .str();

    auto result = execute_command(command);

    return result;
}

// Executes a system command
string wget_rest_client::execute_command(string& command) {

  LOG_VERBOSE({"Executing:\n%1%"} % command)

  auto process_stream = popen(command.c_str(), "r");

  char buffer[128];

  if(process_stream == NULL) {

    throw std::exception();
  }

  ostringstream osstream;

  while (fgets(buffer, sizeof (buffer), process_stream)) {

    osstream << buffer;
  }

  auto output = osstream.str();

  LOG_VERBOSE({"Output:\n%1%"} % output)

  pclose(process_stream);

  return output;
}

// Converts JSON to a property tree
boost::property_tree::ptree wget_rest_client::convert_json_to_property_tree(string& json) {

  stringstream sstream;

  sstream << json;
  
  boost::property_tree::ptree ptree;

  boost::property_tree::read_json(sstream, ptree);

  return ptree;
}
