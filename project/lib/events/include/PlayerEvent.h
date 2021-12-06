#pragma once

#include <boost/json.hpp>
#include "GlobalEnvironment.h"
#include "plug.h"

namespace ClientServer {
    enum {
        tCheck,
        tWalk,
        tShoot,
        tRegister,
    };

    struct Event {
        int info[2];
        int type;
        value serialize() {
            value jv = {
                {"hp", hp},
                {"type", type},
            };
            return jv;
        }

        int deserialize(const std::string & request) {
            value jv = parse(request);
            object const& obj = jv.as_object();
            extract( obj, hp, "hp" );
            extract( obj, type, "type" );
        }
    };
}