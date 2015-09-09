#include <QApplication>
#include <QDir>
#include <QProcess>
#include <QQmlApplicationEngine>
#include <QString>
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
                     QStringList() << "--no-daemon"
                                   << "--port=8080"
                                   << QString("--map=%1").arg(QDir(CUTE_MAP_ROOT).filePath("data/liechtenstein-latest.osm")));

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    DownloadManager downloadManager;
    downloadManager.append(QStringLiteral("http://download.geofabrik.de/europe/liechtenstein-latest.osm.bz2"));
    downloadManager.append(QStringLiteral("http://download.geofabrik.de/europe/liechtenstein-latest.osm.pbf"));
    downloadManager.append(QStringLiteral("http://download.geofabrik.de/europe/liechtenstein-latest.shp.zip"));

    return app.exec();
}
