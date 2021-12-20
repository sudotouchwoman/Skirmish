#include "gtest/gtest.h"
#include "shapes.hpp"

using namespace core;

TEST(CoreCollisionTest, AABBtoAABB) {
    AABB a;
    AABB b(0.0, 0.0, 10.0, 10.0);
    AABB c(5.0, 5.0, 1.0, 3.0);

    // check how shapes collide without knowing
    // each other
    auto intersects = b.IntersectsWith(c);

    EXPECT_TRUE(intersects);
    EXPECT_EQ(intersects.localA, vec2());
    EXPECT_EQ(intersects.localB, vec2());
    EXPECT_EQ(intersects.normal, vec2(1.0, 0.0));
    EXPECT_DOUBLE_EQ(intersects.penetration, 0.5);

    // and backwards
    intersects = c.IntersectsWith(b);

    EXPECT_TRUE(intersects);
    EXPECT_EQ(intersects.localA, vec2());
    EXPECT_EQ(intersects.localB, vec2());
    EXPECT_EQ(intersects.normal, vec2(-1.0, 0.0));
    EXPECT_DOUBLE_EQ(intersects.penetration, 0.5);

    // assert that the results
    // remain the same when shapes
    // know each other
    // this trick is done by inversing the contact
    // point data
    intersects = c.IntersectsWithAABB(b);

    EXPECT_TRUE(intersects);
    EXPECT_EQ(intersects.localA, vec2());
    EXPECT_EQ(intersects.localB, vec2());
    EXPECT_EQ(intersects.normal, vec2(-1.0, 0.0));
    EXPECT_DOUBLE_EQ(intersects.penetration, 0.5);
}

TEST(CoreCollisionTest, AABBtoCircle) {
    AABB a(0.0, 0.0, 2.0, 2.0);
    Circle c(2.0, 1.0, 1.2);

    auto intersects = a.IntersectsWith(c);

    // check without explicitly telling the figure
    // who is this
    EXPECT_TRUE(intersects);
    EXPECT_EQ(intersects.normal, vec2(1.0, 0.0));
    EXPECT_DOUBLE_EQ(intersects.penetration, 0.2);
    EXPECT_EQ(intersects.localA, vec2());
    EXPECT_EQ(intersects.localB, vec2(-1.2, 0.0));
    
    // make sure the explicit method works aswell
    intersects = a.IntersectsWithCircle(c);

    EXPECT_TRUE(intersects);
    EXPECT_EQ(intersects.normal, vec2(1.0, 0.0));
    EXPECT_DOUBLE_EQ(intersects.penetration, 0.2);
    EXPECT_EQ(intersects.localA, vec2());
    EXPECT_EQ(intersects.localB, vec2(-1.2, 0.0));

    // reverse check
    intersects = c.IntersectsWith(a);

    EXPECT_TRUE(intersects);
    EXPECT_EQ(intersects.normal, vec2(-1.0, 0.0));
    EXPECT_DOUBLE_EQ(intersects.penetration, 0.2);
    EXPECT_EQ(intersects.localA, vec2(-1.2, 0.0));
    EXPECT_EQ(intersects.localB, vec2());


    Circle gcc(3.0, 1.0, 1.5);

    intersects = a.IntersectsWith(gcc);

    EXPECT_FALSE(intersects);
}

TEST(CoreCollisionTest, collidingCircles) {
    Circle cc(2.0, 1.0, 1.2);
    Circle cpp(2.0, 1.0, 1.2);

    auto intersects = cpp.IntersectsWith(cc);
    EXPECT_TRUE(intersects);
    EXPECT_DOUBLE_EQ(intersects.penetration, 2*1.2);

    // check intersection of the object with itself
    // nans appear! this is an interesting problem
    // what should one expect when checking the object with itself?
    // the collision is obviuos, but how should we resolve?
    // UPD:
    // in case of collision with itself,
    // return false anyway
    // another idea is checking the geometry
    // equivalence before applying anything
    // (possibly causing nans)
    // actually, this would only require
    // asking if center_ == other.center_?
    EXPECT_FALSE(cc.IntersectsWith(cc));

    // this example is okay now
    // the circles are located closely,
    // but not in the same place!
    Circle gcc(3.0, 1.0, 1.5);
    // private check
    intersects = cc.IntersectsWith(gcc);

    EXPECT_TRUE(intersects);
    EXPECT_DOUBLE_EQ(intersects.penetration, 1.7);
    EXPECT_EQ(intersects.localA, vec2(1.2, 0.0));
    EXPECT_EQ(intersects.localB, vec2(-1.5, 0.0));
    EXPECT_EQ(intersects.normal, vec2(1.0, 0.0));

    // public check
    intersects = cc.IntersectsWithCircle(gcc);

    EXPECT_TRUE(intersects);
    EXPECT_DOUBLE_EQ(intersects.penetration, 1.7);
    EXPECT_EQ(intersects.localA, vec2(1.2, 0.0));
    EXPECT_EQ(intersects.localB, vec2(-1.5, 0.0));
    EXPECT_EQ(intersects.normal, vec2(1.0, 0.0));

    // reverse check
    intersects = gcc.IntersectsWith(cc);

    EXPECT_TRUE(intersects);
    EXPECT_DOUBLE_EQ(intersects.penetration, 1.7);
    EXPECT_EQ(intersects.localA, vec2(-1.5, 0.0));
    EXPECT_EQ(intersects.localB, vec2(1.2, 0.0));
    EXPECT_EQ(intersects.normal, vec2(-1.0, 0.0));

    Circle go(5.0, 5.0, 0.2);
    intersects = go.IntersectsWith(gcc);

    EXPECT_FALSE(intersects);
}


int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}