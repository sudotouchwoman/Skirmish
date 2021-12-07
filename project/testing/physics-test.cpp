#include "gtest/gtest.h"
#include "physical.hpp"

using namespace core;
using namespace physical;

TEST(PhysicalTest, objectConstruction) {
    PhysicalObject hand(10.0);

    // state checks
    EXPECT_DOUBLE_EQ(10.0, hand.getState().inverse_mass);
    EXPECT_EQ(hand.getState().acceleration, vec2());
    EXPECT_EQ(hand.getState().velocity, vec2());
    
    // ensure the unique_ptr is set up finely
    EXPECT_FALSE(hand.hasGeometry());
    EXPECT_EQ(hand.getGeometry(), nullptr);

    // try to pass the geometry from this scope
    // to the hand; make sure the ownership has changed
    auto some_geometry = std::unique_ptr<IShape>(new Circle(0.0, 0.0, 5.0));
    hand.setGeometry(std::move(some_geometry));

    EXPECT_TRUE(hand.hasGeometry());
    EXPECT_NE(hand.getGeometry(), nullptr);

    EXPECT_FALSE(some_geometry);
}

TEST(PhysicalTest, objectCollisions) {
    PhysicalObject hand(10.0);
    PhysicalObject leg(14.0);

    EXPECT_FALSE(hand.hasGeometry());
    EXPECT_FALSE(leg.hasGeometry());
    EXPECT_FALSE(hand.collidesWith(leg));

    auto hand_geometry = std::unique_ptr<IShape>(new AABB(3.0, 3.0, 2.0, 2.0));
    auto leg_geometry = std::unique_ptr<IShape>(new Circle(4.0, 3.0, 0.5));

    hand.setGeometry(std::move(hand_geometry));
    leg.setGeometry(std::move(leg_geometry));

    EXPECT_TRUE(hand.hasGeometry());
    EXPECT_TRUE(leg.hasGeometry());

    EXPECT_FALSE(hand_geometry);
    EXPECT_FALSE(leg_geometry);

    auto collision_details = PhysicalObject::collide(hand, leg);

    EXPECT_TRUE(collision_details);
    EXPECT_DOUBLE_EQ(collision_details.penetration, 0.5);
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
