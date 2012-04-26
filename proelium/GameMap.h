#ifndef MAP_H
#define MAP_H

#include <QObject>
#include "mapsquare.h"
#include "stuff/consts.h"
#include "stuff/abstractions.h"

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
    public:
        virtual void accept(MoveVisitor&)=0;
        int targetX, targetY;
        Unit *attacker;
        MoveResult(int x,int y,Unit *a) : targetX(x), targetY(y), attacker(a) {}
    };
    class SimpleMove: public MoveResult {
    public:
        SimpleMove(int x, int y, Unit *a): MoveResult(x,y,a) { }
        virtual void accept(MoveVisitor &v) { v.visit(*this); }
    };

    class NoMove: public MoveResult {
    public:
        NoMove(int x=-1,int y=-1,Unit*a=NULL) : MoveResult(x,y,NULL) {
            Unit* b = (Unit*)a; b++; // to disable warning
        }
        virtual void accept(MoveVisitor &v) { v.visit(*this); }
    };
    class FightMove: public MoveResult {
    public:
        Unit *defender;
        bool attackerWon;
        MapSquare* defSquare;

        FightMove(Unit *att, Unit *def, bool result, int defx=-1, int defy=-1,
                  MapSquare* defsq=NULL)
            : MoveResult(defx,defy,att), defender(def),
            attackerWon(result), defSquare(defsq) {}

        FightMove(const FightMove& m) : MoveResult(m.targetX,m.targetY,m.attacker) {
            defender = m.defender;
            attackerWon = m.attackerWon;
            defSquare = m.defSquare;
            targetX = m.targetX;
            targetY = m.targetY;
        }
        virtual void accept(MoveVisitor &v) { v.visit(*this); }
    };
}

#define FORALL_UNITS(s, code) {\
	MapSquare* sq;\
	for (int i=0; i<s; ++i)\
		for (int j=0; j<s; ++j) {\
			if ((sq = _field[i][j]) != NULL)\
			foreach (Unit* u, sq->units) {\
				code\
			}\
	}\
}

class GameMap : public QObject {
    Q_OBJECT
private:
    int _width, _height;
    MapSquare*** _field;

    int _lastId;
public:
    int arrSize() { return 2+_width+_height; }
    bool** field_mask;
    explicit GameMap(int,int,QObject *parent = 0);
    MapSquare* getSquare1(int i,int j);
    inline int width() { return _width; }
    inline int height() { return _height; }
    void init();

	void unitsByNation(QVector<Triple<Unit*,int,int> > &ans, int own) {
		MapSquare* sq;
		int s = arrSize();
		for (int i=0; i<s; ++i)
		for (int j=0; j<s; ++j) {
			if ((sq = _field[i][j]) == NULL) continue;
			foreach (Unit* u, sq->units)
				if (u->ownerId == own)
					ans.push_front(Triple<Unit*,int,int>(u,i,j));
		}
	}

	MapSquare* locateUnit(int& i, int& j, Unit* uu) {
		MapSquare* sq;
		int s = arrSize();
		for (i=0; i<s; ++i)
		for (j=0; j<s; ++j) {
			if ((sq = _field[i][j]) == NULL) continue;
			foreach (Unit* u, sq->units)
				if (u->id == uu->id)
					return sq;
		}
		return NULL;
	}

	void visitUnits(UnitVisitor &v) {
		int s = arrSize();
		FORALL_UNITS(s, v.visit(u);)
	}

	Unit* findUnit(int id) {
		MapSquare* sq;
		int s  = arrSize(),i,j;
		for (i=0; i<s; ++i)
		for (j=0; j<s; ++j)	{
			if ((sq = _field[i][j]) == NULL) continue;
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
