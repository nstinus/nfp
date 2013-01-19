#include <gtest/gtest.h>

#include "core/Rating.hpp"

using namespace NFP;


TEST(Date, DateS2US) {
  EXPECT_EQ(utils::DateS2US("2005-07-05"), 2063);
}

TEST(Date, DateUS2S) {
  EXPECT_EQ(utils::DateUS2S(2063), "2005-07-05");
}

TEST(Rating, movie_id) {
  model::Rating r;
  // First value (actually first is 1)
  EXPECT_EQ((unsigned int)0, r.movie_id());
  // Last value
  r.set_movie_id((uint16_t)17770);
  EXPECT_EQ((unsigned int)17770, r.movie_id());
  // Max resolution
  unsigned int max = (0x1 << 15) - 1;
  r.set_movie_id(max);
  EXPECT_EQ(max, r.movie_id());
}

TEST(Rating, user_id) {
  model::Rating r;
  // First value
  EXPECT_EQ((uint32_t)0, r.user_id());
  // Last value
  r.set_user_id((uint32_t)2649429);
  EXPECT_EQ((uint32_t)2649429, r.user_id());
  // Max resolution
  unsigned int max = (0x1 << 24) - 1;
  r.set_user_id(max);
  EXPECT_EQ(max, r.user_id());
}

TEST(Rating, rate) {
  model::Rating r;
  EXPECT_EQ((uint16_t)0, r.rate());
  for (uint16_t i=0; i<6; ++i) {
    r.set_rate(i);
    EXPECT_EQ(i, r.rate());
    EXPECT_EQ((uint8_t)i, r.raw_rate());
  }
}

TEST(Rating, date) {
  model::Rating r;
  // First value
  EXPECT_EQ(0, r.raw_date());
  EXPECT_EQ("1999-11-11", r.date());
  // Last value
  r.set_date((uint16_t)2182);
  EXPECT_EQ((uint16_t)2182, r.raw_date());
  EXPECT_EQ("2005-11-01", r.date());
  // Max resolution
  unsigned int max = (0x1 << 12) - 1;
  r.set_date(max);
  EXPECT_EQ(max, r.raw_date());
}

TEST(Rating, to_string) {
  NFP::model::Rating r(1234, 456, 3, "2005-07-05");
  EXPECT_EQ(r.to_string(), "01234  00000456  3  2005-07-05");
}


int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
