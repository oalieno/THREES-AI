#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../board.h"

using ::testing::ElementsAreArray;

class BoardTest : public ::testing::Test {
    protected:
    void SetUp () override {
        board.value = {3, 2, 3, 0,
                       0, 1, 2, 3,
                       3, 3, 2, 1,
                       0, 1, 0, 6};
    }

    Board board;
};

TEST_F(BoardTest, SlideUp) {
    board.slide(0);
    EXPECT_THAT(board.value, ElementsAreArray({3, 3, 3, 3,
                                               3, 3, 2, 1,
                                               0, 1, 2, 6,
                                               0, 0, 0, 0}));
}

TEST_F(BoardTest, SlideRight) {
    board.slide(1);
    EXPECT_THAT(board.value, ElementsAreArray({0, 3, 2, 3,
                                               0, 0, 3, 3,
                                               0, 3, 3, 3,
                                               0, 0, 1, 6}));
}

TEST_F(BoardTest, SlideDown) {
    board.slide(2);
    EXPECT_THAT(board.value, ElementsAreArray({0, 0, 0, 0,
                                               3, 3, 3, 3,
                                               0, 3, 2, 1,
                                               3, 1, 2, 6}));
}

TEST_F(BoardTest, SlideLeft) {
    board.slide(3);
    EXPECT_THAT(board.value, ElementsAreArray({3, 2, 3, 0,
                                               1, 2, 3, 0,
                                               4, 2, 1, 0,
                                               1, 0, 6, 0}));
}

TEST_F(BoardTest, Score) {
    EXPECT_EQ(board.score(), 96);
}

TEST_F(BoardTest, TileToValue) {
    EXPECT_EQ(board.tileToValue(0), 0);
    EXPECT_EQ(board.tileToValue(1), 1);
    EXPECT_EQ(board.tileToValue(2), 2);
    EXPECT_EQ(board.tileToValue(3), 3);
    EXPECT_EQ(board.tileToValue(6), 4);
    EXPECT_EQ(board.tileToValue(12), 5);
    EXPECT_EQ(board.tileToValue(24), 6);
    EXPECT_EQ(board.tileToValue(48), 7);
    EXPECT_EQ(board.tileToValue(96), 8);
    EXPECT_EQ(board.tileToValue(192), 9);
    EXPECT_EQ(board.tileToValue(384), 10);
    EXPECT_EQ(board.tileToValue(768), 11);
    EXPECT_EQ(board.tileToValue(1536), 12);
    EXPECT_EQ(board.tileToValue(3072), 13);
    EXPECT_EQ(board.tileToValue(6144), 14);
}

TEST_F(BoardTest, ValueToTile) {
    EXPECT_EQ(board.valueToTile(0), 0);
    EXPECT_EQ(board.valueToTile(1), 1);
    EXPECT_EQ(board.valueToTile(2), 2);
    EXPECT_EQ(board.valueToTile(3), 3);
    EXPECT_EQ(board.valueToTile(4), 6);
    EXPECT_EQ(board.valueToTile(5), 12);
    EXPECT_EQ(board.valueToTile(6), 24);
    EXPECT_EQ(board.valueToTile(7), 48);
    EXPECT_EQ(board.valueToTile(8), 96);
    EXPECT_EQ(board.valueToTile(9), 192);
    EXPECT_EQ(board.valueToTile(10), 384);
    EXPECT_EQ(board.valueToTile(11), 768);
    EXPECT_EQ(board.valueToTile(12), 1536);
    EXPECT_EQ(board.valueToTile(13), 3072);
    EXPECT_EQ(board.valueToTile(14), 6144);
}

TEST_F(BoardTest, ValueToScore) {
    EXPECT_EQ(board.valueToScore(0), 0);
    EXPECT_EQ(board.valueToScore(1), 0);
    EXPECT_EQ(board.valueToScore(2), 0);
    EXPECT_EQ(board.valueToScore(3), 3);
    EXPECT_EQ(board.valueToScore(4), 9);
    EXPECT_EQ(board.valueToScore(5), 27);
    EXPECT_EQ(board.valueToScore(6), 81);
    EXPECT_EQ(board.valueToScore(7), 243);
    EXPECT_EQ(board.valueToScore(8), 729);
    EXPECT_EQ(board.valueToScore(9), 2187);
    EXPECT_EQ(board.valueToScore(10), 6561);
    EXPECT_EQ(board.valueToScore(11), 19683);
    EXPECT_EQ(board.valueToScore(12), 59049);
    EXPECT_EQ(board.valueToScore(13), 177147);
    EXPECT_EQ(board.valueToScore(14), 531441);
}
