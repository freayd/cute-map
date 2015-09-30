#ifndef CUTEMAP_H
#define CUTEMAP_H

#include <QApplication>

class DownloadManager;
class TileRenderer;
class QProcess;
class QQmlApplicationEngine;
class QString;

class CuteMap : public QApplication
{
    Q_OBJECT

public:
    CuteMap(int &argc, char **argv);

private slots:
    void initRenderer();
    void setRenderer(const QString &name);

private:
    QQmlApplicationEngine *m_qmlEngine;
    QObject *m_qmlRoot;
    DownloadManager *m_downloadManager;
    TileRenderer *m_tileRenderer;
    QProcess *m_processRenderer;
};

#endif // CUTEMAP_H
