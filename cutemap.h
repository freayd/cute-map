#ifndef CUTEMAP_H
#define CUTEMAP_H

#include <QApplication>

class DownloadManager;
class QQmlApplicationEngine;

class CuteMap : public QApplication
{
    Q_OBJECT

public:
    CuteMap(int &argc, char **argv);

private slots:
    void startRenderer();

private:
    QQmlApplicationEngine *m_qmlEngine;
    DownloadManager *m_downloadManager;
};

#endif // CUTEMAP_H
