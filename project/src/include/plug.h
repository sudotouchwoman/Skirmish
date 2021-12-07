#pragma once

#include "GlobalEnvironment.h"
#include <vector>
#include <string>

class Collision;

class PhisicsManager {
public:
    std::vector<Collision> makePhisics(GameEntities::GlobalEnvironment &) {};
};

class Collision {
public:
    size_t getCollisionType() { return collision_type; }
    GameEntities::GameObject &getMainCollisiant() { return main; }
private:
    GameEntities::GameObject &main;  // main object, wich need to be processed by server
    GameEntities::GameObject &second;
    size_t collision_type;
};

class CollisionManager {
public:
    void resolveCollisions(const std::vector <Collision> & collisions, GameEntities::GlobalEnvironment & environment) {
        for (auto collision : collisions){
            size_t type = collision.getCollisionType();
            switch (type){

            }
        }
    }
private:
    int collisionProcessorByType(size_t type);
};

class PhysicalObject{

};

class Snapshot{
public:
    Snapshot() = default;
    void generateSnapshot(const GameEntities::GlobalEnvironment & ge);
    Snapshot(std::string);
    operator std::string();
    GameEntities::GlobalEnvironment & generateGlobalEnvironmentFromSnapshot();
private:
    std::string _snapshot;
};