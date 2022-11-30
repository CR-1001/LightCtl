/*
 * This file is part of LightCtl.
 * Copyright (C) 2022 Christian Rauch
 *
 * Distributed under terms of the GPL3 license.
 */

// Represents the state of a device or device group. 

#include <string>
#include <optional>
#include <boost/format.hpp>

#include "state.hpp"

using namespace std;

// JSON-serializes the attributes
string state::to_json() {

  auto attributes = (boost::format{"\"on\":%1%"} % (on ? "true" : "false")).str();

  if (on) {

    if (hue65535 != 65536) {
      attributes += (boost::format{", \"hue\":%1%"} % hue65535).str();
    }

    if (sat254 != 255) {
      attributes += (boost::format{", \"sat\":%1%"} % sat254).str();
    }

    if (bri254 != 255) {
      attributes += (boost::format{", \"bri\":%1%"} % bri254).str();
    }
  }

  return (boost::format{"{ %1% }"} % attributes).str();
}

optional<uint> state::get_hue360() {
  if(hue65535 == 65536) return std::nullopt;
  return scale(360, 65535, hue65535);
}

optional<ushort> state::get_sat100() {
  if(sat254 == 255) return std::nullopt;
  return scale(100, 254, sat254);
}

optional<ushort> state::get_bri100() {
  if(bri254 == 255) return std::nullopt;
  return scale(100, 254, bri254);    
}

void state::set_hue360(uint val360) {
  hue65535 = scale(65535, 360, val360);
}

void state::set_sat100(ushort val100) {
  sat254 = scale(254, 100, val100);
}

void state::set_bri100(ushort val100) {
  bri254 = scale(254, 100, val100);
}

// Scales the values (0..360 to 0..65535 and 0..100 to 0..254)
int state::scale(float scale, float max, int value) {

  auto result = value / (max / scale);

  return result;
}