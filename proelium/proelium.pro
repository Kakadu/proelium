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
    reshelpers/gametextureitem.cpp \
    reshelpers/container.cpp \
    reshelpers/anigroup.cpp
HEADERS += \
    framecollection.h \
    unit.h \
    GameMap.h \
    mapsquare.h \
    reshelpers/resloader1.h \
    mapdrawer.h \
    action/action.h \
    reshelpers/rescontainer.h \
    reshelpers/gametextureitem.h \
    reshelpers/anigroup.h \
    GlobalConst.h \
    textures/AbstractTextureItem.h \
    textures/UnitTextureItem.h \
    textures/TerrainTextureItem.h \
    action/UserActionHypervisor.h
FORMS +=

RESOURCES += \
    mainres.qrc

QMAKE_CXXFLAGS += -std=c++0x
