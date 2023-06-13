#include "locationvalues.h"

Control* LocationValues::control[2] = {new GuiControl(), NULL};
MyData* LocationValues::data[2] ={ new FileData(), NULL};
Board* LocationValues::board  = NULL;
shpForm* LocationValues::ShpForm = NULL;

