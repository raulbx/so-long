#pragma once

#include <stddef.h>
#include <stdint.h>

inline uint32_t fake_millis = 0;

inline uint32_t millis() {
  return fake_millis;
}
