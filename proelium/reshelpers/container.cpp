#include "rescontainer.h"

QMap<QString, SpritesPack*> Sprites;

QPair<int,int> directions[8] = {
    QPair<int,int>( 1, 0),  //NE
    QPair<int,int>( 1, 1),  //E
    QPair<int,int>( 0, 1),  //SE
    QPair<int,int>(-1, 1),  //S
    QPair<int,int>( -1,0),  //SW
    QPair<int,int>(-1,-1),  //W
    QPair<int,int>( 0,-1),  //NW
    QPair<int,int>( 1,-1)   //N
};

