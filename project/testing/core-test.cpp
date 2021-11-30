#include <iostream>
#include <sstream>
#include <cassert>

#include "entity.hpp"

using core::allclose;

void test_vec2(std::istream & in, std::ostream & out) {
    try {
        core::vec2 a(5.0, 2.6);
        assert(a.x == 5.0);
        assert(a.y == 2.6);

        core::vec2 b;
        assert(b.x == 0.0);
        assert(b.y == 0.0);

        auto c = a + b;
        assert(c.x == a.x);
        assert(c.y == a.y);

        auto d = a - a;
        assert(d.x != b.x);
        assert(d.y == b.y);

        a += a;
        assert(a.x == 10.0);
        assert(a.y == 5.2);
    }
    catch(const std::exception& e) {
        out << e.what() << '\n';
    }

    out << "core::vec2 test passed, OK\n";
}

void test_AABB(std::istream & in, std::ostream & out) {
    try {
        core::AABB a;
        assert(a.GetCenter().x == 0.0);
        assert(a.GetCenter().y == 0.0);

        assert(a.TopLeft().x == 0.0);
        assert(a.TopLeft().y == 0.0);

        core::AABB b(core::vec2(10.0, 5.0), 2.0, 2.0);

        auto center = b.GetCenter();
        assert(allclose(center.x, 10.0));
        assert(allclose(center.y, 5.0));

        auto tl = b.TopLeft();
        assert(allclose(tl.x, 9.0));
        assert(allclose(tl.y, 6.0));

        auto tr = b.TopRight();
        assert(allclose(tr.x, 11.0));
        assert(allclose(tr.y, 6.0));

        auto bl = b.BottomLeft();
        assert(allclose(bl.x, 9.0));
        assert(allclose(bl.y, 4.0));

        auto br = b.BottomRight();
        assert(allclose(br.x, 11.0));
        assert(allclose(br.y, 4.0));

        auto collides = b.IntersectsWith(a);
        assert(collides == false);
    }
    catch(const std::exception& e) {
        out << e.what() << '\n';
    }

    out << "core::AABB test passed, OK\n";
}

void test_Circle(std::istream & in, std::ostream & out) {
    try {
        core::AABB a(core::vec2(2.0, 3.0), 2.0, 1.0);
        core::Circle circle(a.GetCenter(), 2.0);

        assert(allclose(circle.GetRadius(), 2.0));
        assert(allclose(circle.GetCenter().x, 2.0));
        assert(allclose(circle.GetCenter().y, 3.0));

        assert(a.IntersectsWith(circle) == true);
        assert(circle.IntersectsWith(a) == true);

        core::Circle another_circle(core::vec2(-2.0, -3.0), 2.0);
        assert(circle.IntersectsWith(another_circle) == false);
        assert(a.IntersectsWith(another_circle) == false);
    }
    catch(const std::exception& e) {
        out << e.what() << '\n';
    }

    out << "core::Circle test passed, OK\n";
}

void test_Point(std::istream & in, std::ostream & out) {
    try {
        core::Point p1;
        assert(allclose(p1.GetCenter().x, 0.0));
        assert(allclose(p1.GetCenter().y, 0.0));

        core::Point p2(core::vec2(1.0, 0.0));
        assert(allclose(p2.GetCenter().x, 1.0));
        assert(allclose(p2.GetCenter().y, 0.0));

        assert(p1.IntersectsWith(p2) == false);
        
        core::AABB rect(core::vec2(), 2.0, 2.0);
        assert(rect.IntersectsWith(p1) == true);
        assert(rect.IntersectsWith(p2) == true);

        core::Circle circle(core::vec2(), 0.5);
        assert(circle.IntersectsWith(p1) == true);
        assert(circle.IntersectsWith(p2) == false);
    }
    catch(const std::exception& e) {
        out << e.what() << '\n';
    }

    out << "core::Point test passed, OK\n";
}

int main(int argc, char *argv[]) {
    test_vec2(std::cin, std::cout);
    test_AABB(std::cin, std::cout);
    test_Circle(std::cin, std::cout);
    test_Point(std::cin, std::cout);
    return EXIT_SUCCESS;
}
