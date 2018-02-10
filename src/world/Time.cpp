//
// Created by Milan van Zanten on 09.02.18.
//

#include "Time.h"

void Time::changeSpeed(float targetSpeed) {
    timeSpeed = targetSpeed;
}

void Time::tick(long long int delta) {
    time.deltaTime = static_cast<long long int>(delta * timeSpeed);
    time.timeSinceEpoch += time.deltaTime;
}

long long int Time::timeSinceEpoch() const {
    return time.timeSinceEpoch;
}

long long int Time::deltaTime() const {
    return time.deltaTime;
}

float Time::speed() const {
    return timeSpeed;
}