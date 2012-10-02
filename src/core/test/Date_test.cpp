#include <gtest/gtest.h>

#include "Date.h"


static const std::pair<std::string, unsigned int> dates[] = {
  {"1970-01-01", 2440588},
  {"2005-07-05", 2453557},
  {"2012-02-29", 2455987},
  {"2005-07-05", 2453557},
  {"2005-07-05", 2453557},
};
static const size_t dates_len = sizeof(dates)/sizeof(std::pair<std::string, unsigned int>);

TEST(Date, toJulianDay) {
  for (size_t i=0; i<dates_len; ++i) {
    EXPECT_EQ(NFP::utils::toJulianDay(dates[i].first),
              dates[i].second);
  }
}

TEST(Date, fromJulianDay) {
  for (size_t i=0; i<dates_len; ++i) {
    EXPECT_EQ(NFP::utils::fromJulianDay(dates[i].second), dates[i].first);
  }
}


int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
