#include "fps.hpp"

void FPS::Set(int fps) {
    timePerTick = 1000 / (float) fps;
    lastTime = (float) SDL_GetTicks();
    lastCheck = lastTime;
}

float FPS::Update() {
    currentTime = (float) SDL_GetTicks();
    delta += (currentTime - lastTime) / timePerTick;
    lastTime = currentTime;
    return delta;
}

void FPS::Release() {
    delta = 0;
}

int FPS::InverseTimeCheck() {
    currentCheck = (float) SDL_GetTicks();
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
