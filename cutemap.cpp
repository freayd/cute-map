#include "cutemap.h"

#include "downloadmanager.h"

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

    QObject *qmlRoot = m_qmlEngine->rootObjects().first();
    QObject::connect(m_downloadManager, SIGNAL(progress(QVariant, QVariant, QVariant)),
                     qmlRoot, SLOT(updateStatus(QVariant, QVariant, QVariant)));
    QObject::connect(m_downloadManager, SIGNAL(finished()),
                     this, SLOT(startRenderer()));
    QObject::connect(m_downloadManager, SIGNAL(finished()),
                     qmlRoot, SLOT(showMap()));

    m_downloadManager->append(QStringLiteral("http://download.geofabrik.de/europe/liechtenstein-latest.osm.bz2"));
    m_downloadManager->append(QStringLiteral("http://download.geofabrik.de/europe/liechtenstein-latest.osm.pbf"));
    m_downloadManager->append(QStringLiteral("http://download.geofabrik.de/europe/liechtenstein-latest.shp.zip"));
}

void CuteMap::startRenderer()
{
    QDir memphisDir(MEMPHIS_ROOT);
    QProcess *tileServer = new QProcess(this);
    // tileServer->setProcessChannelMode(QProcess::ForwardedChannels);
    tileServer->setWorkingDirectory(memphisDir.filePath("demos"));
    tileServer->start(memphisDir.filePath("demos/tile-server"),
                      QStringList() << "--no-daemon"
                                    << "--port=8080"
                                    << QString("--map=%1").arg(QDir(CUTE_MAP_ROOT).filePath("data/liechtenstein-latest.osm")));
}
