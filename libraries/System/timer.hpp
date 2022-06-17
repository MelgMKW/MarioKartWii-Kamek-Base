#pragma once
#include <Kamek/kamek.hpp>

class Timer {
public:
  Timer(); // 8051c374
  // vtable 808b2d44
  virtual ~Timer(); // 8051c334
  u16 minutes;
  u8 seconds;
  // padding 0x7
  u16 milliseconds;
  Timer operator -(Timer *other);
  u8 unknown_0xa[2];
}; // Total size 0xc


