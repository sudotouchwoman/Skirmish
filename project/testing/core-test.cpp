#include <iostream>
#include <sstream>
#include <cassert>

#include "core.hpp"

void test_dot2(std::istream & in, std::ostream & out) {
    core::dot2 a(5.0, 2.6);
    assert(a.x() == 5.0);
    assert(a.y() == 2.6);

    core::dot2 b;
    assert(b.x() == 0.0);
    assert(b.y() == 0.0);

    auto c = a + b;
    assert(c.x() == a.x());
    assert(c.y() == a.y());

    auto d = a - a;
    assert(d.x() == b.x());
    assert(d.y() == b.y());

    a += a;
    assert(a.x() == 10.0);
    assert(a.y() == 5.2);

    out << "core::dot2 test passed, OK\n";
}

void test_vec2(std::istream & in, std::ostream & out) {
    core::dot2 p(1, 1);
    core::dot2 m(10, 5);
    core::vec2 a;
    core::vec2 b(p, m);

    assert(a.dst().x() == core::dot2().x());
    assert(a.dst().y() == core::dot2().y());

    assert(b.dst().x() == m.x());
    assert(b.dst().y() == m.y());

    assert(b.src().x() == p.x());
    assert(b.src().y() == p.y());

    out << "core::vec2 test passed, OK\n";
}


int main(int argc, char *argv[]) {
    test_dot2(std::cin, std::cout);
    test_vec2(std::cin, std::cout);
    return EXIT_SUCCESS;
}
