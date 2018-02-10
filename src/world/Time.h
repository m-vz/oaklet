//
// Created by Milan van Zanten on 09.02.18.
//

#ifndef BESTEST_GAME_TIME_H
#define BESTEST_GAME_TIME_H

struct FrameTime {
    long long int timeSinceEpoch;
    long long int deltaTime;
};

class Time {
public:
    /// Change the virtual speed at which time progresses in the world instantly.
    /// \param targetSpeed The new speed.
    void changeSpeed(float targetSpeed);
    /// Tick the world on for a specific amount of *real* time. Updates in-world delta and time since epoch.
    /// \param delta The *real* time that passed during the last tick.
    void tick(long long int delta);
    long long int timeSinceEpoch() const;
    long long int deltaTime() const;
    float speed() const;

private:
    float timeSpeed = 1.0f;
    FrameTime time = {0, static_cast<long long int>(timeSpeed)};
};

#endif //BESTEST_GAME_TIME_H
