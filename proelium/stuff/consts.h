#ifndef CONSTS_H
#define CONSTS_H

#pragma once

#include <QPair>
#include <QString>

namespace Game {
    enum Direction {
        Dir_SW = 1,
        Dir_S, Dir_SE,
        Dir_W = 4,
        Dir_E = 6,
        Dir_NW, Dir_N, Dir_NE
    };
    enum Nations {
        Russia = 0,
        Barbarians = 100
    };
}


#endif // CONSTS_H
