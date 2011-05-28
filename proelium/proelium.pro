# -------------------------------------------------
# Project created by QtCreator 2011-01-03T15:12:49
# -------------------------------------------------
QT += opengl \
    qt3support
TARGET = proelium
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    framecollection.cpp \    
    GameMap.cpp \
    mapsquare.cpp \
    reshelpers/resloader1.cpp \
    mapdrawer.cpp \
    action/action.cpp \
    reshelpers/rescontainer.cpp \
    reshelpers/gametextureitem.cpp \
    reshelpers/container.cpp \
    reshelpers/anigroup.cpp
HEADERS += mainwindow.h \
    framecollection.h \
    unit.h \
    GameMap.h \
    mapsquare.h \
    reshelpers/resloader1.h \
    mapdrawer.h \
    action/action.h \
    reshelpers/rescontainer.h \
    reshelpers/gametextureitem.h \
    reshelpers/container.h \
    reshelpers/anigroup.h \
    GlobalConst.h
FORMS += mainwindow.ui

RESOURCES += \
    mainres.qrc
