#include <sv/resource/DateTime.h>

TEST(DateTime, Operators) {
    sv::DateTime d1;
    sv::DateTime d2;

    EXPECT_TRUE(d1 == d2);
    EXPECT_FALSE(d1 < d2);
    EXPECT_FALSE(d1 > d2);
    EXPECT_TRUE(d1 <= d2);
    EXPECT_TRUE(d1 >= d2);

    sv::DateTime d3(0, 1, 2, 3, 4, 5);
    sv::DateTime d4(1, 2, 3, 4, 5, 6);

    EXPECT_FALSE(d3 == d4);
    EXPECT_TRUE(d3 < d4);
    EXPECT_FALSE(d3 > d4);
    EXPECT_TRUE(d3 <= d4);
    EXPECT_FALSE(d3 >= d4);

    sv::DateTime d5(1, 0, 0, 16, 3, 1994);
    sv::DateTime d6(0, 0, 0, 16, 3, 1994);

    EXPECT_FALSE(d5 == d6);
    EXPECT_FALSE(d5 < d6);
    EXPECT_TRUE(d5 > d6);
    EXPECT_FALSE(d5 <= d6);
    EXPECT_TRUE(d5 >= d6);

    d5 = sv::DateTime(0, 1, 0, 16, 3, 1994);
    d6 = sv::DateTime(59, 0, 0, 16, 3, 1994);

    EXPECT_FALSE(d5 == d6);
    EXPECT_FALSE(d5 < d6);
    EXPECT_TRUE(d5 > d6);
    EXPECT_FALSE(d5 <= d6);
    EXPECT_TRUE(d5 >= d6);

    d5 = sv::DateTime(0, 0, 1, 16, 3, 1994);
    d6 = sv::DateTime(60, 59, 0, 16, 3, 1994);

    EXPECT_FALSE(d5 == d6);
    EXPECT_FALSE(d5 < d6);
    EXPECT_TRUE(d5 > d6);
    EXPECT_FALSE(d5 <= d6);
    EXPECT_TRUE(d5 >= d6);

    d5 = sv::DateTime(0, 0, 1, 16, 3, 1994);
    d6 = sv::DateTime(60, 59, 0, 16, 3, 1994);

    EXPECT_FALSE(d5 == d6);
    EXPECT_FALSE(d5 < d6);
    EXPECT_TRUE(d5 > d6);
    EXPECT_FALSE(d5 <= d6);
    EXPECT_TRUE(d5 >= d6);

    d5 = sv::DateTime(0, 0, 0, 17, 3, 1994);
    d6 = sv::DateTime(60, 59, 23, 16, 3, 1994);

    EXPECT_FALSE(d5 == d6);
    EXPECT_FALSE(d5 < d6);
    EXPECT_TRUE(d5 > d6);
    EXPECT_FALSE(d5 <= d6);
    EXPECT_TRUE(d5 >= d6);

    d5 = sv::DateTime(0, 0, 0, 0, 4, 1994);
    d6 = sv::DateTime(60, 59, 23, 31, 3, 1994);

    EXPECT_FALSE(d5 == d6);
    EXPECT_FALSE(d5 < d6);
    EXPECT_TRUE(d5 > d6);
    EXPECT_FALSE(d5 <= d6);
    EXPECT_TRUE(d5 >= d6);

    d5 = sv::DateTime(0, 0, 0, 0, 0, 1995);
    d6 = sv::DateTime(60, 59, 23, 31, 11, 1994);

    EXPECT_FALSE(d5 == d6);
    EXPECT_FALSE(d5 < d6);
    EXPECT_TRUE(d5 > d6);
    EXPECT_FALSE(d5 <= d6);
    EXPECT_TRUE(d5 >= d6);
}
