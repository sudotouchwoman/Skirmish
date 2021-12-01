#include "GameObjects.h"
//#include "GameLoop.h"
//#include "GlobalEnvironment.h"
//#include "GameServer.h"
#include <iostream>
#include <boost/json.hpp>

int main(){
    GameEntities::Player aboba;
    aboba.getHP();
    boost::json::value jv = {{"1", "lakdsjfl"}};
    boost::json::object jo = jv.as_object();
    std::cout << jo.at("1");
    return 0;
}