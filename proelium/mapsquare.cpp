#include "mapsquare.h"

void MapSquare::addUnit(Unit* unit) {
    units.append(unit);
}
void MapSquare::removeUnit(Unit* u) {
    int x  = units.lastIndexOf(u);
    if (x>=0)
	units.removeAt(x);
}


MapSquare::MapSquare()
{
       units.clear();
}
