/*
 * This file is part of LightCtl.
 * Copyright (C) 2022 Christian Rauch
 *
 * Distributed under terms of the GPL3 license.
 */

// Represents the state of a device or device group. 

#ifndef __STATE_H__
#define __STATE_H__

class state {

  public:

  // Unique identifier (set by the bridge)
  uint id = 0;

  // Name (as configured by the user)
  std::string name = "";
  
  // On or off
  bool on = false;
  
  // Single device or group
  bool group = false;

  // Member IDs (if it is a group)
  std::vector<uint> members;

  // Hue: 0..65535, 65536 = not set
  uint hue65535 = 65536;
  
  // Saturation: 0..254, 255 = not set
  ushort sat254 = 255;
  
  // Brightness: 0..254, 255 = not set
  ushort bri254 = 255;

  // JSON-serializes the attributes
  std::string to_json();

  std::optional<uint> get_hue360();

  std::optional<ushort> get_sat100();

  std::optional<ushort> get_bri100();

  void set_hue360(uint val360);

  void set_sat100(ushort val100);

  void set_bri100(ushort val100);

  // Scales the values (0..360 to 0..65535 and 0..100 to 0..254)
  int scale(float scale, float max, int value);
};

#endif