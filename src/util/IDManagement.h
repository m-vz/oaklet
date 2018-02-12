//
// Created by milan on 06.10.2017.
//

#ifndef OAKLET_IDMANAGEMENT_H
#define OAKLET_IDMANAGEMENT_H

#include <cstdint>

class IDManagement {
public:
    /// Generate a new unique ID.
    /// \return The new ID.
    static uint32_t generateID();

private:
    static uint32_t lastID;
};

#endif //OAKLET_IDMANAGEMENT_H
