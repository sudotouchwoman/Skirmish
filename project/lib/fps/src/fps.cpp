#include "fps.hpp"

int FPS::fps = 0, FPS::startInverseTimer = 0;
double FPS::delta = 0, FPS::currentTime = 0, FPS::lastTime = 0, FPS::timePerTick = 0, FPS::lastCheck = 0, FPS::currentCheck = 0;

void FPS::Set(int required_fps) {
    fps = required_fps;
    timePerTick = (double) 1000 / fps;
    lastTime = SDL_GetTicks();
    lastCheck = lastTime;
}

double FPS::Update() {
    currentTime = SDL_GetTicks();
    delta += (currentTime - lastTime) / timePerTick;
    lastTime = currentTime;
    return delta;
}

void FPS::Release() {
    delta = 0;
}

int FPS::InverseTimeCheck() {
    currentCheck = SDL_GetTicks();
    if ((currentCheck - lastCheck) / 1000 >= 1) {
        lastCheck = currentCheck;
        return --startInverseTimer;
    }
    else
        return startInverseTimer;
}

void FPS::SetInverseTimer(int seconds) {
    startInverseTimer = seconds;
}
