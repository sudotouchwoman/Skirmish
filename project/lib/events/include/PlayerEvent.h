#pragma once

namespace ClientServer {
    enum Type{
        tCheck,
        tWalk,
        tShoot,
        tRegister,
    };

    enum Movement{
        U, L, R, D, UL, UR, DL, DR, Void,
    };

    struct ShootEvent{
        int angle;
        int weapon;
    };

    struct MoveEvent{
        int movement;
    };

    struct InteractEvent{
        int object_type;
    };
}