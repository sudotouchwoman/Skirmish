#pragma once

#include <string>

// main game parameters
const int game_tick = 10; // miliseconds
const double physics_tick = static_cast<double>(game_tick) / 1000;
const int bullet_speed = 800;
const int player_speed = 4;
const double default_player_radius = 4;
const double default_bullet_radius = 1;
const float default_spawn_x = 0;
const float default_spawn_y = 0;
const float default_spawn_vx = 0;
const float default_spawn_vy = 0;

// main game parameters

// client server settings
const std::string host_server = "127.0.0.1";
const std::string port_server = "5000";
const int udp_size = 65527;

const int max_transfer_event_bytes = 40;
// client server settings


// textures settings
const size_t size_of_textures = 8;
// textures settings
