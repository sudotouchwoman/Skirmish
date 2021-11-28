#pragma once

#include <boost/json.hpp>

using namespace boost::json;

namespace GameEntities {
    class GameObject {

    };

    class Player : public GameObject {
    };

//    void tag_invoke(value_from_tag, value &jv, Player const &p) {
//        jv = {
//            {"id", c.id},
//            {"name", c.name},
//            {"current", c.current}
//        };
//    }
//
//    Player tag_invoke( value_to_tag< Player >, value const& jv )
//    {
//        object const& obj = jv.as_object();
//        return Player {
//            value_to<int>( obj.at( "id" ) ),
//            value_to<std::string>( obj.at( "name" ) ),
//            value_to<bool>( obj.at( "current" ) )
//        };
//    }

    class Bullet : public GameObject {

    };

    class Terrain : public GameObject {

    };
}
