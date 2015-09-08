#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>

#include <QDir>
#include <QFile>
#include <QProcess>
#include <QQueue>
#include <QUrl>

class QNetworkAccessManager;
class QNetworkReply;

class DownloadManager : public QObject
{
    Q_OBJECT

public:
    DownloadManager(QObject *parent = 0);

    void append(const QUrl &url);

private slots:
    void downloadNext();
    void progress(qint64 received, qint64 total);
    void write();
    void downloaded();
    void extracted(int exitCode);
    void extractionError(QProcess::ProcessError);

private:
    QNetworkAccessManager *m_networkManager;
    QNetworkReply *m_reply;
    QProcess *m_extractionProcess;
    QQueue<QUrl> m_queue;
    const QDir m_outputDir;
    QFile m_output;
};

#endif // DOWNLOADMANAGER_H
