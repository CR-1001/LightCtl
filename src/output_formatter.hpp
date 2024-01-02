/*
 * This file is part of LightCtl.
 * Copyright (C) 2022 Christian Rauch
 *
 * Distributed under terms of the GPL3 license.
 */

// Prints the device infos and the help.

#ifndef __OUTPUTFORMATTER_H__
#define __OUTPUTFORMATTER_H__

#include <iostream>
#include <string>
#include <bits/stdc++.h>
#include <map>
#include <boost/format.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "state.hpp"

using namespace std;

class output_formatter{

  public:

    // Prints the device states
    static void print_state(bool raw, bool brief, vector<state>& states, string& json);

    // Prints the info
    static void print_info();

    // Prints the error message
    static void print_error();

    // Prints the help
    static void print_help();

    // Format value
    template<typename t>
    static string format_fallback(std::optional<t>& value, const string& fallback);
};

#endif