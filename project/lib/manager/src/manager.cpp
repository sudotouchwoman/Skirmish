#include "manager.hpp"

void GameManager::Run(size_t port_client) {
    GameEntities::GlobalEnvironment ge;
    Client::ConnectionClient cc(port_client);
    cc.setSnapshotRecieveCallback([&ge](std::string &&s){ge.handleServerResponse(std::move(s));});

    Menu menu(&window, frames);
    std::string name;
    int texture_id;
    menu.Load(name, texture_id);
    size_t player_id = cc.registerPlayer(name.c_str(), texture_id);
    for (int i = 0; i < 10000000; ++i) {}
    cc.sendEvent();

    EventManager command(&window);
    Camera camera(&window, player_id);
    fps.SetInverseTimer(inverseTimeSeconds);
    fps.Set(frames);

    window.SetCursor(window.texture.textureImageMap[Tile::GAME_CURSOR]);
    fps.SetInverseTimer(inverseTimeSeconds);
    while (!command.HandleEvents(cc, ge, enable)) {
        if (fps.Update() >= 1) {
            window.ClearSurface();
            inverseTimeSeconds = fps.InverseTimeCheck();
            if (inverseTimeSeconds >= 0)
                menu.InverseTimer(inverseTimeSeconds);
            else
                enable = true;
            camera.Update(ge.getPlayers(), ge.getBullets());
            window.Render();
            fps.Release();
        }
    }
}