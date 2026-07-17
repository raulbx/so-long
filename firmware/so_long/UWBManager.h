#pragma once

#include <Arduino.h>

class UWBManager {
 public:
  bool begin();
  bool update();
  float latestDistanceMeters() const;

 private:
  float latestDistanceMeters_ = -1.0f;
  float filteredDistanceMeters_ = -1.0f;
};
