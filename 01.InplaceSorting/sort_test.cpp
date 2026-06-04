#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "sorting.h"

TEST(SortTest, BubbleSort)
{
    std::vector<int> data = {4, 2, 5, 1, 3};
    std::vector<int> expected = {1, 2, 3, 4, 5};
    bubble_sort(data.begin(), data.end());
    EXPECT_EQ(data, expected);
}

TEST(SortTest, QuickSort)
{
    std::vector<int> data = {4, 2, 5, 1, 3};
    std::vector<int> expected = {1, 2, 3, 4, 5};
    quick_sort(data.begin(), data.end());
    EXPECT_EQ(data, expected);
}

TEST(SortTest, EmptyAndSingle)
{
    std::vector<int> empty;
    quick_sort(empty.begin(), empty.end());
    EXPECT_TRUE(empty.empty());

    std::vector<int> single = {1};
    bubble_sort(single.begin(), single.end());
    EXPECT_EQ(single[0], 1);
}
