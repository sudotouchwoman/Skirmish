#ifndef TIMER_H
#define TIMER_H

#include <SDL.h>
#include <iostream>

class FPS {
public:
    void Set(int fps);
    float Update();
    void Release();
    int InverseTimeCheck();
    void SetInverseTimer(int seconds);

    FPS(const FPS &other) = delete;
    FPS &operator=(const FPS &other) = delete;
    FPS() : startInverseTimer(0), delta(0), currentTime(0), lastTime(0), timePerTick(0), lastCheck(0), currentCheck(0) {}

private:
    int startInverseTimer;
    float delta, currentTime, lastTime, timePerTick, lastCheck, currentCheck;

};

#endif //TIMER_H
