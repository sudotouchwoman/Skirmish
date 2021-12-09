#include "PlayerEvent.h"
#include <iostream>

int main(){
    ClientServer::MoveEvent ev {ClientServer::DR};
    std::cout << reinterpret_cast<char * const>(&ev);

}