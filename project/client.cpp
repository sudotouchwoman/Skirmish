#include "PlayerEvent.h"
#include <iostream>
#include <string>
#include "ConnectionClient.h"
#include "GlobalEnvironment.h"

int main(int argc, char *argv[]){
    std::ios_base::sync_with_stdio(false);
    GameEntities::GlobalEnvironment ge;

    Client::ConnectionClient cc(strtoul(argv[1], nullptr, 10));
    cc.setSnapshotRecieveCallback([&ge](std::string &&s){ge.handleServerResponse(std::move(s));});

    int ev;
    while(std::cin >> ev){
        switch (ev) {
            case 1: cc.registerPlayer("aboba", 1); break;
            case 2: {// move
                int type;
                std::cin >> type;
                ClientServer::MoveEvent ev_send{type};
                cc.sendEvent(ev_send);
                break;
            }
            case 3: {// shoot
                float x, y;
                int weapon;
                std::cin >> x >> y >> weapon;
                ClientServer::ShootEvent ev_send{x, y, weapon};
                cc.sendEvent(ev_send);
                break;
            }
            case 4:
                cc.sendEvent();
                break;
            default:break;
        }
    }
    return 0;
}