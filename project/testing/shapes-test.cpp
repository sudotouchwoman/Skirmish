#include <vector>
#include <memory>
#include <algorithm>

#include "gtest/gtest.h"
#include "shapes.hpp"

using namespace core;

typedef std::unique_ptr<IShape> IShapeUPtr;

TEST(CoreShapeTest, construction) {
    AABB a;
    auto a_center = a.GetCenter();

    EXPECT_EQ(a.Type(), AABB_shape);
    EXPECT_DOUBLE_EQ(a_center.x, 0.0);
    EXPECT_DOUBLE_EQ(a_center.y, 0.0);

    Circle c;
    auto c_center = c.GetCenter();

    EXPECT_EQ(c.Type(), Circle_shape);
    EXPECT_DOUBLE_EQ(c.GetRadius(), 0.0);
    EXPECT_DOUBLE_EQ(c_center.x, 0.0);
    EXPECT_DOUBLE_EQ(c_center.y, 0.0);

    EXPECT_EQ(a_center, c_center);

    Point p;
    auto p_center = p.GetCenter();

    EXPECT_EQ(p.Type(), Point_shape);
    EXPECT_DOUBLE_EQ(p_center.x, 0.0);
    EXPECT_DOUBLE_EQ(p_center.y, 0.0);

    AABB aabb(4.0, 2.0, 4.0, 3.0);
    auto aabb_center = aabb.GetCenter();

    EXPECT_NE(aabb_center, a_center);
    EXPECT_NE(aabb_center, c_center);

    EXPECT_DOUBLE_EQ(aabb_center.x, 4.0);
    EXPECT_DOUBLE_EQ(aabb_center.y, 2.0);
}

TEST(CoreShapeTest, intersections) {
    AABB a;
    AABB b(0.0, 0.0, 10.0, 10.0);

    EXPECT_TRUE(b.IntersectsWith(a));
    EXPECT_TRUE(a.IntersectsWith(b));

    AABB c(5.0, 5.0, 3.0, 3.0);

    EXPECT_TRUE(b.IntersectsWith(c));
    EXPECT_TRUE(c.IntersectsWith(b));

    EXPECT_FALSE(a.IntersectsWith(c));
    EXPECT_FALSE(c.IntersectsWith(a));

    Circle d(5.0, 7.0, 3.0);

    EXPECT_TRUE(d.IntersectsWith(c));
    EXPECT_TRUE(c.IntersectsWith(d));

    EXPECT_FALSE(d.IntersectsWith(a));
    EXPECT_FALSE(a.IntersectsWith(d));

    Circle e(10.0, 12.0, 1.0);

    EXPECT_FALSE(e.IntersectsWith(d));
    EXPECT_FALSE(d.IntersectsWith(e));

    EXPECT_FALSE(e.IntersectsWith(b));
    EXPECT_FALSE(b.IntersectsWith(e));
    EXPECT_FALSE(e.IntersectsWith(c));
    EXPECT_FALSE(c.IntersectsWith(e));

    Point p(10.0, 12.0);

    EXPECT_TRUE(p.IntersectsWith(e));
    EXPECT_TRUE(e.IntersectsWith(p));

    EXPECT_FALSE(p.IntersectsWith(d));
    EXPECT_FALSE(d.IntersectsWith(p));

    EXPECT_FALSE(p.IntersectsWith(c));
    EXPECT_FALSE(c.IntersectsWith(p));

    EXPECT_FALSE(p.IntersectsWith(a));
    EXPECT_FALSE(a.IntersectsWith(p));
}

TEST(CoreShapeTest, polymorphism) {
    std::vector<IShapeUPtr> shapes;
    shapes.push_back(std::make_unique<AABB>(0.0, 2.0, 4.0, 3.0));
    shapes.push_back(std::make_unique<Circle>(0.0, 1.5, 1.0));
    shapes.push_back(std::make_unique<Point>(3.0, 3.0));

    auto any_intersects = std::any_of(
            shapes.begin(),
            shapes.end(),
            [&shapes](const IShapeUPtr & p) {
                return std::any_of(
                    shapes.begin(),
                    shapes.end(),
                    [&shapes, &p](const IShapeUPtr & m) {
                        if (m == p) return false;
                        return p->IntersectsWith(*m.get());
                    });
                });

    EXPECT_TRUE(any_intersects);

    auto all_intersect = std::all_of(
            shapes.begin(),
            shapes.end(),
            [&shapes](const IShapeUPtr & p) {
                return std::all_of(
                    shapes.begin(),
                    shapes.end(),
                    [&shapes, &p](const IShapeUPtr & m) {
                        if (m == p) return false;
                        return p->IntersectsWith(*m.get());
                    });
                });

    EXPECT_FALSE(all_intersect);
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}