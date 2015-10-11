TEMPLATE = app

QT += qml quick widgets location network

HEADERS += \
    cutemap.h \
    downloadmanager.h \
    tilerenderer.h \
    tilerenderer.hpp \
    mapserverrenderer.h \
    mapnikrenderer.h

SOURCES += main.cpp \
    cutemap.cpp \
    downloadmanager.cpp \
    tilerenderer.cpp \
    mapserverrenderer.cpp \
    mapnikrenderer.cpp

RESOURCES += qml.qrc

# Default rules for deployment.
include(deployment.pri)

DEFINES += \
    CUTEMAP_DIR=\"\\\"$${_PRO_FILE_PWD_}\\\"\" \
    MAPNIK_DIR=\"\\\"$${OUT_PWD}/3rdparty\\\"\" \
    MAPSERVER_DIR=\"\\\"$${_PRO_FILE_PWD_}/3rdparty/mapserver\\\"\" \
    MEMPHIS_DIR=\"\\\"$${_PRO_FILE_PWD_}/3rdparty/memphis\\\"\" \
    P7ZIP_DIR=\"\\\"$${_PRO_FILE_PWD_}/3rdparty/p7zip\\\"\"

QMAKE_CXX = $$system($${OUT_PWD}/3rdparty/bin/mapnik-config --cxx)
QMAKE_LINK = $$system($${OUT_PWD}/3rdparty/bin/mapnik-config --cxx)
QMAKE_CXXFLAGS += $$system($${OUT_PWD}/3rdparty/bin/mapnik-config --cxxflags --includes --dep-includes --defines)
QMAKE_LFLAGS += $$system($${OUT_PWD}/3rdparty/bin/mapnik-config --libs --ldflags --dep-libs)
