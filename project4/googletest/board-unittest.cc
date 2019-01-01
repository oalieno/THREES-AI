#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../board.h"

class BoardTest : public ::testing::Test {
    protected:
    void SetUp () override {
        board.value = {3, 2, 3, 0,
                       0, 1, 2, 3,
                       3, 3, 2, 1,
                       0, 1, 0, 0};
    }

    Board board;
};

TEST(BoardTest, Slide) {
    EXPECT_EQ(1, 1);
}
