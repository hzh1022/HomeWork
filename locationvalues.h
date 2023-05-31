#ifndef LOCATIONVALUES_H
#define LOCATIONVALUES_H

#include "data.h"
#include "control.h"
#include "board.h"

class LocationValues
{
public:
    static Control *control[2];
    static MyData *data[2];
    static Board *board;
};


#endif // LOCATIONVALUES_H
