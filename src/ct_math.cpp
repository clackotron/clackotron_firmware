#include "ct_math.h"

uint8_t CTMath::bcd2dec(uint8_t bcd) {
  return (bcd >> 4) * 10 + (bcd & 0x0F);
}

uint8_t CTMath::dec2bcd(uint8_t dec) {
  return ((dec / 10) << 4) + (dec % 10);
}
