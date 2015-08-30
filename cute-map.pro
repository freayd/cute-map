TEMPLATE = app

QT += qml quick widgets location

SOURCES += main.cpp

RESOURCES += qml.qrc

# Default rules for deployment.
include(deployment.pri)

DEFINES += MEMPHIS_ROOT=\"\\\"$$clean_path($${_PRO_FILE_PWD_}/3rdparty/memphis)\\\"\"
