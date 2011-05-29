#include "rescontainer.h"
#include "action/UserActionHypervisor.h"

QMap<QString, SpritesPack*> Sprites;
UserActionHyperVisor* MainHyperVisor;

QPair<int,int> directions[10] = {
    QPair<int,int>( 0, 0),  // unused
    QPair<int,int>(-1, 0),  //SW
    QPair<int,int>(-1, 1),  //S
    QPair<int,int>( 0, 1),  //SE
    QPair<int,int>(-1,-1),  //W
    QPair<int,int>( 0, 0),  // unused
    QPair<int,int>( 1, 1),  //E
    QPair<int,int>( 0,-1),  //NW
    QPair<int,int>( 1,-1),  //N
    QPair<int,int>( 1, 0)   //NE
};
