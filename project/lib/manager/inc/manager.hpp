#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <memory>

#include "menu.hpp"
#include "command.hpp"

class Manager {
public:
    static void Run();

private:
    static const int fps;
};

#endif //MANAGER_HPP