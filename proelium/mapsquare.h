#ifndef MAPSQUARE_H
#define MAPSQUARE_H

#include <QObject>

class MapSquare : public QObject
{
Q_OBJECT
private:
    int _temp;
public:
    explicit MapSquare();
    void set(int x) {
	_temp=x;
    }
    int get() { return _temp; }

signals:

public slots:

};

#endif // MAPSQUARE_H
