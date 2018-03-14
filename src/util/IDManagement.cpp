//
// Created by milan on 06.10.2017.
//

#include "IDManagement.h"

uint32_t IDManagement::lastID = 0;

uint32_t IDManagement::generateID() {
    return ++lastID;
}
