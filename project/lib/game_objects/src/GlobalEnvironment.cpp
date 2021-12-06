#include "GlobalEnvironment.h"

using namespace boost::json;
using GameEntities::GlobalEnvironment;

void GlobalEnvironment::updateEnvironment() {

}
int GlobalEnvironment::getAccess() {
    _mutex.lock();
    return 0;
}
int GlobalEnvironment::finishAccess() {
    _mutex.unlock();
    return 0;
}

int GlobalEnvironment::deleteObjects(){
    for (auto elem: _game_objects){
        if (elem.deleted){
            elem = _game_objects[_game_objects.size() - 1];
            _game_objects.pop_back();
        }
    }
    return 0;
}

int GlobalEnvironment::addObject(const std::shared_ptr<GameEntities::GameObject> &go){
    _game_objects.push_back(go);
    return 0;
}

std::vector<std::shared_ptr<GameEntities::GameObject>> &GlobalEnvironment::getModifyGameObjects() {
    return _game_objects;
}

int GlobalEnvironment::generateSnapshot(){
    /// not effective realization!
    array arr;
    for (size_t i = 0; i < _game_objects.size(); i++){
        arr.emplace_back(_game_objects[i]->serialize());
    }
    _snapshot = serialize(arr);
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
    /// definetely memory loss coused by shared_ptr
    array arr = parse(_snapshot).as_array();
    _game_objects.resize(arr.size());
    for (size_t i = 0; i < _game_objects.size(); i++)
        _game_objects[i]->deserialize(arr[i]);
    return 0;
}