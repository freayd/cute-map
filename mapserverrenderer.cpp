#include "mapserverrenderer.h"

#include <QDir>
#include <QFile>
#include <QProcess>
#include <QString>
#include <QTextStream>

MapServerRenderer::MapServerRenderer(const QString &mapPath, QObject *parent)
    : TileRenderer(parent)
{
    QFile pathFile(QDir(CUTE_MAP_ROOT).filePath("data/mapserver-path.map"));
    if (pathFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        QTextStream(&pathFile) << "SHAPEPATH \"" << mapPath << "\"\n";
    else
        qWarning("Unable to write file '%s'.", qUtf8Printable(pathFile.fileName()));
}

QByteArray MapServerRenderer::render(int x, int y, int z)
{
    QString mapFile = QDir(CUTE_MAP_ROOT).filePath("data/mapserver.map");
    QString query = QString("map=%1&mode=tile&tile=%2+%3+%4").arg(mapFile).arg(x).arg(y).arg(z);

    QProcess server;
    server.start(QDir(MAPSERVER_ROOT).filePath("build/mapserv"),
                 QStringList() << "-nh" << QString("QUERY_STRING=%1").arg(query));

    if (server.waitForFinished())
        return server.readAll();
    else
        return QByteArray();
}
