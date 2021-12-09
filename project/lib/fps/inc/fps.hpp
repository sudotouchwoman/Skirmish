#ifndef TIMER_H
#define TIMER_H

#include <SDL.h>
#include <iostream>

class FPS {
public:
    static void Set(int fps);
    static double Update();
    static void Release();
    static int InverseTimeCheck();
    static void SetInverseTimer(int seconds);

private:
    static int fps, startInverseTimer;
    static double delta, currentTime, lastTime, timePerTick, lastCheck, currentCheck;

};

#endif //TIMER_H
