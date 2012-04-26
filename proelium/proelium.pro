# -------------------------------------------------
# Project created by QtCreator 2011-01-03T15:12:49
# -------------------------------------------------
QT += opengl qt3support
TARGET = proelium
TEMPLATE = app
SOURCES += main.cpp \
    framecollection.cpp \
    GameMap.cpp \
    mapsquare.cpp \
    reshelpers/resloader1.cpp \
    mapdrawer.cpp \
    reshelpers/rescontainer.cpp \
    reshelpers/container.cpp \
    stuff/abstractions.cpp \
    forms/mainwindow.cpp \
    action/UserActionHyperVisor.cpp
HEADERS += \
    framecollection.h \
    unit.h \
    GameMap.h \
    mapsquare.h \
    reshelpers/resloader1.h \
    mapdrawer.h \
    action/action.h \
    reshelpers/rescontainer.h \
    GlobalConst.h \
    textures/AbstractTextureItem.h \
    textures/UnitTextureItem.h \
    textures/TerrainTextureItem.h \
    stuff/abstractions.h \
    stuff/consts.h \
    stuff/consts2.h \
    forms/mainwindow.h \
    action/UserActionHyperVisor.h
FORMS += \
    forms/mainwindow.ui

RESOURCES += \
    mainres.qrc

QMAKE_CXXFLAGS += -std=c++0x -Woverloaded-virtual  \
#-Wold-style-cast -Wfloat-equal -Wshadow
