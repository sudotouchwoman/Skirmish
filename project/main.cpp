#include "GameObjects.h"
//#include "GameLoop.h"
#include "GlobalEnvironment.h"
//#include "GameServer.h"
#include <iostream>

int main(){
    GameEntities::Player *aboba;
    std::cout << aboba;

    GameEntities::GlobalEnvironment ge;
    ge.generateSnapshot();
//    Server::GameLoop gl(&ge);
//    Server::GameServer gs;
//    gs.run();
    return 0;
}