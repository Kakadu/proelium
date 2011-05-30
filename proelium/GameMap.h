#ifndef MAP_H
#define MAP_H

#include <QObject>
#include "mapsquare.h"
#include "stuff/consts.h"

namespace Move {
    class SimpleMove;
    class NoMove;
    class FightMove;
    class MoveVisitor {
    public:
        virtual void visit(SimpleMove&)=0;
        virtual void visit(FightMove&)=0;
        virtual void visit(NoMove&)=0;
    };

    class MoveResult {
    //private:
    //    MoveResult() {} // abstract class =)
    public:
        virtual void accept(MoveVisitor&)=0;
    };
    class SimpleMove: public MoveResult {
    public:
        SimpleMove(){}
        virtual void accept(MoveVisitor &v) { v.visit(*this); }
    };

    class NoMove: public MoveResult {
    public:
        NoMove() {}
        NoMove(const NoMove&) { }
        virtual void accept(MoveVisitor &v) { v.visit(*this); }
    };
    class FightMove: public MoveResult {
    public:
        Unit *attacker, *defender;
        bool attackerWon;
        FightMove(Unit *a, Unit *b,bool r): attacker(a), defender(b),
            attackerWon(r) {}
        FightMove(const FightMove& m) {
            attacker = m.attacker;
            defender = m.defender;
            attackerWon = m.attackerWon;
        }
        virtual void accept(MoveVisitor &v) { v.visit(*this); }
    };
}

class GameMap : public QObject {
    Q_OBJECT
private:
    int _width, _height;
    MapSquare*** _field;
    inline int s() { return 2+_width+_height; }
    int _lastId;
public:
    bool** field_mask;
    explicit GameMap(int,int,QObject *parent = 0);
    MapSquare* getSquare1(int i,int j);
    inline int width() { return _width; }
    inline int height() { return _height; }
    void init();

    MapSquare* locateUnit(int& i, int& j, Unit* uu) {
	MapSquare* sq;
	for (i=0; i<s(); ++i)
	    for (j=0; j<s(); ++j) {
	    if ((sq = _field[i][j]) != NULL)
		foreach (Unit* u, sq->units)
		    if (u->id == uu->id)
			return sq;
	}
	return NULL;
    }

    Unit* findUnit(int id) {
	MapSquare* sq;
	for (int i=0; i<s(); ++i)
	    for (int j=0; j<s(); ++j) {
	    if ((sq = _field[i][j]) != NULL)
		foreach (Unit* u, sq->units)
		    if (u->id == id)
			return u;
	}
	return NULL;
    }
    int inline nextId() {
        return _lastId++;
    }
    bool f() { return false; }
    Move::MoveResult *tryMove(Unit*, Game::Direction);
signals:
    void unitRemoved(int);
};

#endif // MAP_H
