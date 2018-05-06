//
// Created by Milan van Zanten on 06.05.18.
//

#ifndef BESTEST_GAME_UPDATEENTITY_H
#define BESTEST_GAME_UPDATEENTITY_H

#include "../Time.h"

class UpdateEntity {
public:
    virtual void update(Time &time) = 0;
};

#endif //BESTEST_GAME_UPDATEENTITY_H
