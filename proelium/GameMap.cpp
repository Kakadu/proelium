#include "GameMap.h"
#include <QVector>
#include <iostream>
#include "mapsquare.h"
using namespace std;

GameMap::GameMap(int w, int h,QObject *parent) : QObject(parent) {
    _width=w;
    _height=h;
    int s  = w+h+2;
    field_mask = new bool*[s];
    _field = new MapSquare**[s];

    for (int i=0;i<s;++i) {
	field_mask[i] = new bool[s];
	_field[i] = new MapSquare*[s];
	for (int j=0; j<s; ++j) {
	    _field[i][j] = NULL;
	    field_mask[i][j] = false;
	}
    }

    for (int i=0;i<s;++i)
	for (int j=0;j<s;++j) {
	    _field[i][j]= NULL;
	    field_mask[i][j] = false;
	}

    //little lines
    for (int i=0; i<h+2; ++i)
	for (int j=0; j<w+1; ++j)
	    field_mask[w+i-j][j+i] = true;
    //large lines
    for (int i=0; i<h+1; ++i)
	for (int j=0; j<w+2; ++j)
	    field_mask[w+1+i-j][j+i] = true;

    for (int i=0;i<s;i++) {
	for (int j=0; j<s; j++) {
	    if (field_mask[i][j]) {
		_field[i][j] = new MapSquare();
		_field[i][j]->set(8);
	    } else
		_field[i][j] = NULL;
	}
    }
    for (int i=0; i<s; ++i) {
	for (int j=0; j<s; ++j) {
	    if (field_mask[i][j])
		std::cout << 8 << " ";
	    else
		std::cout << " . ";
	}
	std::cout<<"\n";
    }
}

MapSquare* GameMap::getSquare1(int i,int j) {
    return _field[i][j];
}
