#pragma once
#include <cgp/cgp.hpp>

#define RGB(R, G, B)                   \
  cgp::vec3 {                          \
    R / 256.0f, G / 256.0f, B / 256.0f \
  }
