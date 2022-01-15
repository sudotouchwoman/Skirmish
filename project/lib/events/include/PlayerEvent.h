#pragma once

namespace ClientServer {
    enum Type{
        CHECK = 1, //for enumeration which can be passed by string (\000 == 0)
        WALK,
        SHOOT,
        ROTATE,
        REGISTER,
    };

    enum Movement{
        U, L, R, D, UL, UR, DL, DR, Void,
    };

    struct ShootEvent{
        float x;
        float y;
        int weapon;
    };

    struct RotateEvent{
        float angle;
    };

    struct MoveEvent{
        int movement;
    };

    struct InteractEvent{
        int object_type;
    };
}