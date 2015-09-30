#include "cutemap.h"

#include "downloadmanager.h"
#include "mapserverrenderer.h"

#include <QDir>
#include <QMetaObject>
#include <QProcess>
#include <QQmlApplicationEngine>
#include <QStandardPaths>
#include <QString>
#include <QStringList>
#include <QUrl>
#include <QVariant>

CuteMap::CuteMap(int &argc, char **argv)
    : QApplication(argc, argv),
      m_qmlEngine(new QQmlApplicationEngine(this)),
      m_downloadManager(new DownloadManager(this)),
      m_tileRenderer(0),
      m_processRenderer(0)
{
    m_qmlEngine->load(QUrl(QStringLiteral("qrc:/main.qml")));
    m_qmlRoot = m_qmlEngine->rootObjects().first();

    connect(m_downloadManager, SIGNAL(progress(QVariant, QVariant, QVariant)),
            m_qmlRoot, SLOT(updateStatus(QVariant, QVariant, QVariant)));
    connect(m_downloadManager, SIGNAL(finished()), SLOT(initRenderer()));

    QStringList countries, extensions;
    countries << "europe/liechtenstein" << "asia/south-korea";
    extensions << "osm.bz2" << "osm.pbf" << "shp.zip";
    for (int c = 0; c < countries.size(); ++c)
        for (int e = 0; e < extensions.size(); ++e)
            m_downloadManager->append(QString("http://download.geofabrik.de/%1-latest.%2")
                                      .arg(countries.at(c))
                                      .arg(extensions.at(e)));

    connect(m_qmlRoot, SIGNAL(rendererChanged(QString)), SLOT(setRenderer(QString)));
}

void CuteMap::initRenderer()
{
    setRenderer("Memphis");
}

void CuteMap::setRenderer(const QString &name)
{
    QDir(QStandardPaths::writableLocation(QStandardPaths::GenericCacheLocation) + QLatin1String("/QtLocation/osm")).removeRecursively();
    if (m_tileRenderer) {
        connect(m_tileRenderer, SIGNAL(finished()), m_tileRenderer, SLOT(deleteLater()));
        m_tileRenderer->terminate();
        m_tileRenderer = 0;
    } else if (m_processRenderer) {
        m_processRenderer->deleteLater();
        m_processRenderer = 0;
    }

    QString mapPath = QDir(CUTE_MAP_ROOT).filePath("data/liechtenstein-latest");
    if (name == QStringLiteral("Memphis")) {
        QDir memphisDir(MEMPHIS_ROOT);
        m_processRenderer = new QProcess(this);
        // m_processRenderer->setProcessChannelMode(QProcess::ForwardedChannels);
        m_processRenderer->setWorkingDirectory(memphisDir.filePath("demos"));
        m_processRenderer->start(memphisDir.filePath("demos/tile-server"),
                                 QStringList() << "--no-daemon"
                                               << "--port=8080"
                                               << QString("--map=%1").arg(mapPath + ".osm"));

        QMetaObject::invokeMethod(m_qmlRoot, "showMap", Q_ARG(QVariant, "http://localhost:8080/"));
    } else if (name == QStringLiteral("MapServer")) {
        m_tileRenderer = new MapServerRenderer(mapPath + ".shp", this);
        connect(m_tileRenderer, SIGNAL(listening(QVariant)), m_qmlRoot, SLOT(showMap(QVariant)));
        m_tileRenderer->start();
    } else {
        qWarning("Unknown renderer '%s'.", qUtf8Printable(name));
    }
}
