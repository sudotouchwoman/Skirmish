#include "GlobalEnvironment.h"

using namespace boost::json;
using GameEntities::GlobalEnvironment;

int GlobalEnvironment::getAccess() {
    _mutex.lock();
    return 0;
}

int GlobalEnvironment::finishAccess() {
    _mutex.unlock();
    return 0;
}

int GlobalEnvironment::deleteObjects(){

    // lambda for elements deletion
    auto deleter = [](auto &Vector) {
        for (auto &elem: Vector){
        if (elem.deleted){
            elem = std::move(Vector[Vector.size() - 1]);
            Vector.pop_back();
        }
    }};

    deleter(Players);
//    deleter(Bullets);
//    deleter(Terrain);
//    deleter(Objects);
}

//int GlobalEnvironment::onCollision(Collision cl){
//    // game logic depending on type
//
//    // calling methods on collision of two passed links of elements
//    return 0;
//}

int GlobalEnvironment::addPlayer(GameEntities::Player & pl){
    Players.emplace_back();
    Players[Players.size() - 1] = std::move(pl);
    return 0;
}

int GlobalEnvironment::generateSnapshot(){
    /// not effective realization!
    auto vectorSerializer = [](auto &Vector){
        array arr;
        for (auto & elem: Vector){
            arr.push_back(elem.serialize());
        }
        return std::move(arr);
    };

    value jv = {
        {"Players", vectorSerializer(Players)},
//        {"Bullets", vectorSerializer(Players)},
//        {"Terrain", vectorSerializer(Players)},
//        {"Objects", vectorSerializer(Players)},
    };

    _snapshot = serialize(jv);
    return 0;
}
const std::string &GlobalEnvironment::getSnapshot(){
    return _snapshot;
}
int GlobalEnvironment::setSnapshot(std::string &&new_snapshot){
    _snapshot = new_snapshot;
    return 0;
}
int GlobalEnvironment::getObjectsFromSnapshot(){
//    /// definetely memory loss coused by shared_ptr
//    array arr = parse(_snapshot).as_array();
//    _game_objects.resize(arr.size());
//    for (size_t i = 0; i < _game_objects.size(); i++)
//        _game_objects[i]->deserialize(arr[i]);
//    return 0;
}