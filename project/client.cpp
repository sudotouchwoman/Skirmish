#include "PlayerEvent.h"
#include <iostream>

int main(){
    ClientServer::MoveEvent ev{ClientServer::DR};
    char* data = reinterpret_cast<char*>(&ev);
    for (int i = 0; i < sizeof(ev); ++i)
        std::cout << std::string(data, sizeof(ev));
}