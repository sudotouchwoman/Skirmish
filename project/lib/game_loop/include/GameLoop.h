#pragma once

namespace Server {
    class GameLoop {
    public:
        void Run();
    private:
        GlobalEnvironment GE;
        void ResolveCollisions();
        void BroadcastSnapshot();
        void SendSnapshot();
        void ReadHandler();
    };
}