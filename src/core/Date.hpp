#ifndef __NFP__DATE_H__
#define __NFP__DATE_H__

#include <math.h>
#include <stdint.h>

namespace NFP {
  namespace utils {
    inline uint32_t toJulianDay(std::string const& d);
    inline std::string fromJulianDay(uint32_t jd);
  }
}


inline uint32_t NFP::utils::toJulianDay(std::string const& d) {
  unsigned int year = 0, month = 0, day = 0;
  if (sscanf(d.c_str(), "%4u-%02u-%02u", &year, &month, &day) != 3) {
    return -1;
  }

  unsigned int a = (14 - month) / 12;
  unsigned int y = year + 4800 - a;
  unsigned int m = month + 12 * a - 3;
  return day + (153*m+2)/5 + 365*y + y/4 - y/100  + y/400 - 32045;
}


inline std::string NFP::utils::fromJulianDay(uint32_t jd) {
  unsigned int j  = jd + 32044;
  unsigned int g  = j / 146097;
  unsigned int dg = j % 146097;
  unsigned int c  = (dg / 36524 + 1) * 3 / 4;
  unsigned int dc = dg - c * 36524;
  unsigned int b  = dc / 1461;
  unsigned int db = dc % 1461;
  unsigned int a  = (db / 365 + 1) * 3 / 4;
  unsigned int da = db - a * 365;
  unsigned int y  = g * 400 + c * 100 + b * 4 + a;
  unsigned int m  = (da * 5 + 308) / 153 - 2;
  unsigned int d  = da - (m + 4) * 153 / 5 + 122;

  unsigned int Y  = y - 4800 + (m + 2) / 12;
  unsigned int M  = (m + 2) % 12 + 1;
  unsigned int D  = d + 1;

  char ret[11];
  sprintf(ret, "%4u-%02u-%02u", Y, M ,D);
  return ret;
}

#endif // __NFP__DATE_H__
