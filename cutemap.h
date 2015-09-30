#ifndef CUTEMAP_H
#define CUTEMAP_H

#include <QApplication>

#include <QString>

class DownloadManager;
class TileRenderer;
class QProcess;
class QQmlApplicationEngine;

class CuteMap : public QApplication
{
    Q_OBJECT

public:
    CuteMap(int &argc, char **argv);

private slots:
    void setCountry(const QString &country);
    void setRenderer(const QString &renderer);
    void initMap();

private:
    void showMap();

    QQmlApplicationEngine *m_qmlEngine;
    QObject *m_qmlRoot;
    DownloadManager *m_downloadManager;
    QString m_country;
    QString m_renderer;
    TileRenderer *m_tileRenderer;
    QProcess *m_processRenderer;
};

#endif // CUTEMAP_H
