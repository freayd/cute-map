#include <QApplication>
#include <QDir>
#include <QProcess>
#include <QQmlApplicationEngine>
#include <QStringList>

#include "downloadmanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QDir memphisDir(MEMPHIS_ROOT);
    QProcess tileServer;
    // tileServer.setProcessChannelMode(QProcess::ForwardedChannels);
    tileServer.setWorkingDirectory(memphisDir.filePath("demos"));
    tileServer.start(memphisDir.filePath("demos/tile-server"),
                     QStringList() << "--no-daemon" << "--port=8080");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    DownloadManager downloadManager;
    downloadManager.append(QStringLiteral("http://download.geofabrik.de/europe/liechtenstein-latest.osm.bz2"));
    downloadManager.append(QStringLiteral("http://download.geofabrik.de/europe/liechtenstein-latest.osm.pbf"));
    downloadManager.append(QStringLiteral("http://download.geofabrik.de/europe/liechtenstein-latest.shp.zip"));

    return app.exec();
}
