TEMPLATE = app

QT += qml quick widgets location network

HEADERS += \
    downloadmanager.h

SOURCES += main.cpp \
    downloadmanager.cpp

RESOURCES += qml.qrc

# Default rules for deployment.
include(deployment.pri)

DEFINES += \
    CUTE_MAP_ROOT=\"\\\"$${_PRO_FILE_PWD_}\\\"\" \
    MEMPHIS_ROOT=\"\\\"$${_PRO_FILE_PWD_}/3rdparty/memphis\\\"\"
