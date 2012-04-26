#include "UserActionHyperVisor.h"

UserActionHyperVisor::UserActionHyperVisor(GameMap* m, QObject *parent) :
    QObject(parent)
{
    _map = m;
    _playableOwners << Game::Russia;
    curPlayer = 0;
}

void inline UserActionHyperVisor::extractUnits(int ownerId) {
    _map->unitsByNation(_unitsList, ownerId);
}
