#ifndef ACTION_H
#define ACTION_H
#include <QObject>
#include "unit.h"

class FireUnitAction;
class MoveUnitAction;
class NoAction;
class EndWarAction;
class NewUnitsAppearedAction;

class UnitVisitor {
public:
    virtual void visit(FireUnitAction&)=0;
    virtual void visit(MoveUnitAction&)=0;
    virtual void visit(EndWarAction&)=0;
    virtual void visit(NoAction&)=0;
    virtual void visit(NewUnitsAppearedAction&)=0;
};
class AbstractUnitAction
{
public:
    AbstractUnitAction() {}
    virtual void accept(UnitVisitor&)=0;
};

class MoveUnitAction : public AbstractUnitAction {
    int _xOld, _yOld, _x, _y;
    Unit* _u;
public:
    Unit* unit() { return _u; }
    int x() { return _x; }
    int y() { return _y; }
    int xOld() { return _xOld; }
    int yOld() { return _yOld; }

    MoveUnitAction(int xo, int yo, int x, int y,Unit* u)
	: _xOld(xo), _yOld(yo), _x(x),_y(y),_u(u) {}
    MoveUnitAction(MoveUnitAction& act) {
	_u = act.unit();
	_x = act.x();
	_y = act.y();
	_xOld = act.xOld();
	_yOld = act.yOld();
    }

    virtual void accept(UnitVisitor &v) {
	v.visit(*this);
    }
};
class FireUnitAction : public AbstractUnitAction {    
public:
    const int attackerID;
    const int victimID;
    QString attackerName;
    QString victimName;
    const bool result; // true if killed
    FireUnitAction(int id, QString aName, int tid, QString vName, bool r) :
	    attackerID(id), victimID(tid),
	    attackerName(aName), victimName(vName), result(r) {}

    virtual void accept(UnitVisitor &v) {
	v.visit(*this);
    }
};

class EndWarAction : public AbstractUnitAction {
public:
    QString message;
    EndWarAction(QString msg) : message(msg) {}
    virtual void accept(UnitVisitor &v) {
	v.visit(*this);
    }
};

class NewUnitsAppearedAction : public AbstractUnitAction {
public:
    NewUnitsAppearedAction() {}
    virtual void accept(UnitVisitor &v) {
	v.visit(*this);
    }
};

class NoAction : public AbstractUnitAction {
public:
    NoAction() {}
    virtual void accept(UnitVisitor &v) {
	v.visit(*this);
    }
};

#endif // ACTION_H
