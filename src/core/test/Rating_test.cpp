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
  model::Rating r(1, 0, 0, "2005-07-05");
  EXPECT_EQ(r.movie_id(), (unsigned int)1);
  r.set_movie_id((uint16_t)17770);
  EXPECT_EQ(r.movie_id(), (unsigned int)17770);
  r.set_movie_id((uint16_t)1234);
  EXPECT_EQ(r.movie_id(), (unsigned int)1234);
  r.set_movie_id((uint16_t)-5);
  EXPECT_NE(r.movie_id(), -5);
}

TEST(Rating, to_string) {
  NFP::model::Rating r(1234, 456, 3, "2005-07-05");
  EXPECT_EQ(r.to_string(), "01234  00000456  3  2005-07-05");
}


int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
