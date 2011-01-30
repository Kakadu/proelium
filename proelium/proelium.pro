# -------------------------------------------------
# Project created by QtCreator 2011-01-03T15:12:49
# -------------------------------------------------
QT += opengl \
    phonon \
    multimedia \
    qt3support
TARGET = proelium
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    framecollection.cpp \
    unit.cpp \
    GameMap.cpp \
    mapsquare.cpp \
    reshelpers/resloader1.cpp \
    reshelpers/terrainitem.cpp \
    mapdrawer.cpp
HEADERS += mainwindow.h \
    framecollection.h \
    unit.h \
    GameMap.h \
    mapsquare.h \
    reshelpers/resloader1.h \
    reshelpers/terrainitem.h \
    mapdrawer.h
FORMS += mainwindow.ui
