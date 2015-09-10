#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>

#include <QDir>
#include <QFile>
#include <QProcess>
#include <QQueue>
#include <QString>
#include <QUrl>
#include <QVariant>

class QNetworkAccessManager;
class QNetworkReply;

class DownloadManager : public QObject
{
    Q_OBJECT

public:
    DownloadManager(QObject *parent = 0);

    void append(const QUrl &url);

signals:
    void progress(const QVariant &text, const QVariant &processed, const QVariant &total);
    void finished();

private slots:
    void downloadNext();
    void downloadProgress(qint64 received, qint64 total);
    void write();
    void downloaded();
    void extracted(int exitCode);
    void extractionError(QProcess::ProcessError);

private:
    void emitProgress(const QString &text, qint64 processed, qint64 total);

    QNetworkAccessManager *m_networkManager;
    QNetworkReply *m_reply;
    QProcess *m_extractionProcess;
    QQueue<QUrl> m_queue;
    const QDir m_outputDir;
    QFile m_output;
    unsigned int m_downloadCount;
    unsigned int m_totalCount;
};

#endif // DOWNLOADMANAGER_H
