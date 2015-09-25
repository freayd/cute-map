#include "cutemap.h"

#include "downloadmanager.h"
#include "mapserverrenderer.h"

#include <QDir>
#include <QProcess>
#include <QQmlApplicationEngine>
#include <QString>
#include <QStringList>
#include <QUrl>

CuteMap::CuteMap(int &argc, char **argv)
    : QApplication(argc, argv),
      m_qmlEngine(new QQmlApplicationEngine(this)),
      m_downloadManager(new DownloadManager(this))
{
    m_qmlEngine->load(QUrl(QStringLiteral("qrc:/main.qml")));
    m_qmlRoot = m_qmlEngine->rootObjects().first();
    connect(m_downloadManager, SIGNAL(progress(QVariant, QVariant, QVariant)),
            m_qmlRoot, SLOT(updateStatus(QVariant, QVariant, QVariant)));
    connect(m_downloadManager, SIGNAL(finished()), SLOT(startRenderer()));

    m_downloadManager->append(QStringLiteral("http://download.geofabrik.de/europe/liechtenstein-latest.osm.bz2"));
    m_downloadManager->append(QStringLiteral("http://download.geofabrik.de/europe/liechtenstein-latest.osm.pbf"));
    m_downloadManager->append(QStringLiteral("http://download.geofabrik.de/europe/liechtenstein-latest.shp.zip"));
}

void CuteMap::startRenderer()
{
    QString mapPath = QDir(CUTE_MAP_ROOT).filePath("data/liechtenstein-latest.shp");

    /*
    QDir memphisDir(MEMPHIS_ROOT);
    QProcess *tileServer = new QProcess(this);
    // tileServer->setProcessChannelMode(QProcess::ForwardedChannels);
    tileServer->setWorkingDirectory(memphisDir.filePath("demos"));
    tileServer->start(memphisDir.filePath("demos/tile-server"),
                      QStringList() << "--no-daemon"
                                    << "--port=8080"
                                    << QString("--map=%1").arg(mapPath));
    */

    TileRenderer *renderer = new MapServerRenderer(mapPath, this);
    connect(renderer, SIGNAL(listening(QVariant)), m_qmlRoot, SLOT(showMap(QVariant)));
    renderer->start();
    connect(this, SIGNAL(aboutToQuit()), renderer, SLOT(terminate()));
}
