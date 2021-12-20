#include "gtest/gtest.h"
#include "geometry.hpp"

using namespace core;

TEST(CoreGeometryTest, construction) {
    const double ax = 5.0;
    const double ay = 2.6;

    vec2 a(ax, ay);
    EXPECT_DOUBLE_EQ(a.x, ax);
    EXPECT_DOUBLE_EQ(a.y, ay);

    vec2 b;
    EXPECT_DOUBLE_EQ(b.x, 0.0);
    EXPECT_DOUBLE_EQ(b.y, 0.0);

    auto c = a + b;
    EXPECT_DOUBLE_EQ(c.x, a.x);
    EXPECT_DOUBLE_EQ(c.y, a.y);

    auto d = a - a;
    EXPECT_DOUBLE_EQ(d.x, 0.0);
    EXPECT_DOUBLE_EQ(d.y, 0.0);

    a += a;
    EXPECT_DOUBLE_EQ(a.x, 2*ax);
    EXPECT_DOUBLE_EQ(a.y, 2*ay);
}

TEST(CoreGeometryTest, magnitude) {
    vec2 a(1.0, 0.0);
    vec2 b(3.0, 4.0);

    EXPECT_DOUBLE_EQ(a.len_squared(), 1.0);
    EXPECT_DOUBLE_EQ(b.len_squared(), 25.0);

    EXPECT_DOUBLE_EQ(a.len(), 1.0);
    EXPECT_DOUBLE_EQ(b.len(), 5.0);

    EXPECT_DOUBLE_EQ(a.distance_to_squared(b), 20.0);
    EXPECT_DOUBLE_EQ(b.distance_to_squared(a), 20.0);

    vec2 a_norm = a.normalized();
    EXPECT_DOUBLE_EQ(a_norm.len(), 1.0);
    EXPECT_DOUBLE_EQ(a_norm.len_squared(), 1.0);
}

TEST(CoreGeometryTest, rotation) {
    vec2 a(1.0, 0.0);
    vec2 b(0.0, 1.0);

    EXPECT_NE(a, b);

    a.rotate_d(90.0);
    EXPECT_DOUBLE_EQ(a.y, 1.0);
    EXPECT_DOUBLE_EQ(a.x, 0.0);

    EXPECT_EQ(a, b);

    a.rotate_d(90.0);
    EXPECT_DOUBLE_EQ(a.y, 0.0);
    EXPECT_DOUBLE_EQ(a.x, -1.0);

    a.rotate_d(180.0);
    EXPECT_DOUBLE_EQ(a.y, 0.0);
    EXPECT_DOUBLE_EQ(a.x, 1.0);
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
