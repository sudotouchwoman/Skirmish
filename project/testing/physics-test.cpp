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
<<<<<<< HEAD
    
    // ensure the unique_ptr is set up finely
    EXPECT_FALSE(hand.hasGeometry());
    EXPECT_EQ(hand.getGeometry(), nullptr);

    // try to pass the geometry from this scope
    // to the hand; make sure the ownership has changed
    auto some_geometry = std::unique_ptr<IShape>(new Circle(0.0, 0.0, 5.0));
    hand.setGeometry(std::move(some_geometry));

    EXPECT_TRUE(hand.hasGeometry());
    EXPECT_NE(hand.getGeometry(), nullptr);
=======

    // ensure the unique_ptr is set up finely
    EXPECT_FALSE(hand.hasGeometry());
    EXPECT_ANY_THROW(hand.getGeometry());

    // try to pass the geometry from this scope
    // to the hand; make sure the ownership has changed
    auto some_geometry = IShapeUPtr(new Circle(0.0, 0.0, 5.0));
    hand.setGeometry(std::move(some_geometry));

    EXPECT_TRUE(hand.hasGeometry());
    EXPECT_NO_THROW(hand.getGeometry());
>>>>>>> 2cb5d489be76079190caa65cc88f6a6f9f5bdfca

    EXPECT_FALSE(some_geometry);
}

TEST(PhysicalTest, objectCollisions) {
    PhysicalObject hand(10.0);
    PhysicalObject leg(14.0);

    EXPECT_FALSE(hand.hasGeometry());
    EXPECT_FALSE(leg.hasGeometry());
    EXPECT_FALSE(hand.collidesWith(leg));

<<<<<<< HEAD
    auto hand_geometry = std::unique_ptr<IShape>(new AABB(3.0, 3.0, 2.0, 2.0));
    auto leg_geometry = std::unique_ptr<IShape>(new Circle(4.0, 3.0, 0.5));
=======
    auto hand_geometry = IShapeUPtr(new AABB(3.0, 3.0, 2.0, 2.0));
    auto leg_geometry = IShapeUPtr(new Circle(4.5, 3.0, 1.0));
>>>>>>> 2cb5d489be76079190caa65cc88f6a6f9f5bdfca

    hand.setGeometry(std::move(hand_geometry));
    leg.setGeometry(std::move(leg_geometry));

<<<<<<< HEAD
    EXPECT_TRUE(hand.hasGeometry());
    EXPECT_TRUE(leg.hasGeometry());

    EXPECT_FALSE(hand_geometry);
    EXPECT_FALSE(leg_geometry);
=======
    ASSERT_TRUE(hand.hasGeometry());
    ASSERT_TRUE(leg.hasGeometry());

    ASSERT_FALSE(hand_geometry);
    ASSERT_FALSE(leg_geometry);
>>>>>>> 2cb5d489be76079190caa65cc88f6a6f9f5bdfca

    auto collision_details = PhysicalObject::collide(hand, leg);

    EXPECT_TRUE(collision_details);
<<<<<<< HEAD
    EXPECT_DOUBLE_EQ(collision_details.penetration, 0.5);
=======
    // penetration in case of AABB+Circle
    // is computed as R - delta, where delta
    // is difference between circle's origin relative
    // to AABB origin and the closest point to the circle
    // on the AABB perimeter
    // this is really easier to show than to explain verbally
    EXPECT_DOUBLE_EQ(collision_details.penetration, 0.5);
    // normal direction is computes as follows:
    // circle's relative position minus closest (clamped) point on AABB
    // e.g., in this case the delta is (1.5, 0)
    // and the closest point on box is (1, 0)
    // the normal direction is (0.5, 0), i.e. the normal itself is (1, 0)
    EXPECT_EQ(collision_details.normal, vec2(1.0, 0.0));
    EXPECT_EQ(collision_details.localA, vec2(0.0, 0.0));
    EXPECT_EQ(collision_details.localB, vec2(-1.0, 0.0));

    // it is important that collide() method updates location!
    // (it tries to separate the objects so that they do not overlap)
    // thus the positions should be reset
    hand_geometry = IShapeUPtr(new Circle(5.5, 4.0, 1.0));
    leg_geometry = IShapeUPtr(new Circle(4.5, 3.0, 1.0));

    hand.setGeometry(std::move(hand_geometry));
    leg.setGeometry(std::move(leg_geometry));

    collision_details = PhysicalObject::collide(leg, hand);

    EXPECT_TRUE(collision_details);
    EXPECT_DOUBLE_EQ(collision_details.penetration, 2.0 - std::sqrt(2.0));
    EXPECT_DOUBLE_EQ(collision_details.normal.angle_d(), 45.0);
    EXPECT_DOUBLE_EQ(collision_details.localA.angle_d(), 45.0);
    EXPECT_DOUBLE_EQ(collision_details.localB.angle_d(), -135.0);

    // objects should be separated so that do not overlap any longer
    collision_details = PhysicalObject::collide(leg, hand);
    EXPECT_FALSE(collision_details);

    // the arguments for AABB creation:
    // (x, y) origin, width, heigth
    hand_geometry = IShapeUPtr(new AABB(-3.0, -5.0, 3.0, 10.0));
    leg_geometry = IShapeUPtr(new AABB(vec2(), 4.0, 2.0));

    hand.setGeometry(std::move(hand_geometry));
    leg.setGeometry(std::move(leg_geometry));

    collision_details = PhysicalObject::collide(hand, leg);
    
    EXPECT_TRUE(collision_details);
    EXPECT_DOUBLE_EQ(collision_details.penetration, 0.5);
    EXPECT_EQ(collision_details.normal, vec2(1.0, 0.0));
    EXPECT_EQ(collision_details.localA, vec2());
    EXPECT_EQ(collision_details.localB, vec2());

    // objects should be separated so that do not overlap any longer
    collision_details = PhysicalObject::collide(leg, hand);
    EXPECT_FALSE(collision_details);
}

TEST(PhysicalTest, objectMovement) {
    PhysicalObject bibwatcher(1.0);
    auto some_geometry = IShapeUPtr(new Circle());

    bibwatcher.setGeometry(std::move(some_geometry));
    ASSERT_TRUE(bibwatcher.hasGeometry());

    // set a valocity value (the default is zeros)
    bibwatcher.getState().velocity = { 2.0, 1.0 };
    ASSERT_EQ(bibwatcher.getState().velocity, vec2(2.0, 1.0));

    // perform step of physical simulation
    // with time step of 1s and make
    // sure that the position of the circle changed appropriately
    bibwatcher.update(1.0);
    EXPECT_EQ(bibwatcher.getGeometry().GetCenter(), vec2(2.0, 1.0));
    
    // make one more tick, check the position again
    // these tests are kinda trivial but can help
    // to ensure that methods implemented do not trigger
    // segfault at least...
    bibwatcher.update(1.0);
    EXPECT_EQ(bibwatcher.getGeometry().GetCenter(), vec2(4.0, 2.0));

    // next step: assign new value to the acceleration
    // vector
    bibwatcher.getState().acceleration = { 0.0, 1.0 };
    ASSERT_EQ(bibwatcher.getState().acceleration, vec2(0.0, 1.0));

    // tick with non-zero acceleration:
    // the vecocity is updated first, then the 
    bibwatcher.update(1.0);
    EXPECT_EQ(bibwatcher.getState().velocity, vec2(2.0, 2.0));
    EXPECT_EQ(bibwatcher.getGeometry().GetCenter(), vec2(6.0, 4.0));
}

TEST(PhysicalTest, objectInteraction) {
    PhysicalObject ball, wall;

    auto ball_geometry = IShapeUPtr(new Circle(vec2(), 1.0));
    auto wall_geometry = IShapeUPtr(new AABB(7.0, 0.0, 4.0, 10.0));

    ball.setGeometry(std::move(ball_geometry));
    wall.setGeometry(std::move(wall_geometry));

    ball.getState().inverse_mass = 10.0;
    wall.getState().inverse_mass = 0.0;

    ball.getState().velocity = { 2.0, 1.0 };

    ball.update(3.0);

    // after 3s of movement with constant speed
    // the ball should be at this position
    ASSERT_EQ(ball.getGeometry().GetCenter(), vec2(6.0, 3.0));

    auto collision_details = PhysicalObject::collide(ball, wall);

    // problem:
    // the ball has been teleported
    // into the wall (its origin is inside the AABB)
    // in this case, no normal direction can be obtained!
    // thus, the two bodies will never be 
    ASSERT_TRUE(collision_details);
    EXPECT_DOUBLE_EQ(collision_details.penetration, 1.0);
    EXPECT_EQ(collision_details.normal, vec2());

    EXPECT_EQ(wall.getGeometry().GetCenter(), vec2(7.0, 0.0));
}

TEST(PhysicalTest, collisionResolution) {
    // now check again, but with lower dt
    // make sure the speed is flipped correctly
    PhysicalObject ball(1.0), wall;

    auto ball_geometry = IShapeUPtr(new Circle(vec2(), 1.0));
    auto wall_geometry = IShapeUPtr(new AABB(7.0, 0.0, 4.0, 10.0));

    ball.setGeometry(std::move(ball_geometry));
    wall.setGeometry(std::move(wall_geometry));

    // regular checks not to run onto nullptr
    ASSERT_TRUE(ball.hasGeometry());
    ASSERT_TRUE(wall.hasGeometry());

    // set the velocity from the previous example
    ball.getState().velocity = { 2.0, 1.0 };

    // make sure that there is no collision before anything moves
    auto collision_details = PhysicalObject::collide(ball, wall);
    ASSERT_FALSE(collision_details);


    // check the positions after 2.25s of movement
    // this number was selected on purpose, as
    // at this very moment the ball will intersect
    // with the AABB but its origin will not yet lie
    // inside the box
    // so that this collision will be resolved successfully
    ball.update(2.25);

    EXPECT_EQ(ball.getGeometry().GetCenter(), vec2(4.5, 2.25));
    EXPECT_EQ(ball.getState().velocity, vec2(2.0, 1.0));

    collision_details = PhysicalObject::collide(ball, wall);

    EXPECT_TRUE(collision_details);
    // the velocity should flip so that the ball bounces
    // from the wall
    EXPECT_EQ(ball.getState().velocity, vec2(-2.0, 1.0));
    // and its position should be adjusted so that
    // it no longer collides with the wall
    // note: well, this readjustment is not the best one,
    // as it has no idea about where did the ball came from,
    // so it only is capable of pushing it in the direction of normal,
    // i.e. along x-axis in this case
    // its y-coordinate, however, does not change and this may look wierd
    // in some cases. I personally hope that the objects, primarily
    // projectiles, will not encounter this issue as fixing it
    // would probably involve rewriting the entire physics lib
    EXPECT_EQ(ball.getGeometry().GetCenter(), vec2(4.0, 2.25));

    // make sure that second collide() call
    // without updating (i.e. moving) objects
    // yields negative result (collision was resolved)
    collision_details = PhysicalObject::collide(ball, wall);
    EXPECT_FALSE(collision_details);
>>>>>>> 2cb5d489be76079190caa65cc88f6a6f9f5bdfca
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
