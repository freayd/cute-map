#include "downloadmanager.h"

#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>

DownloadManager::DownloadManager(QObject *parent)
    : QObject(parent),
      m_networkManager(new QNetworkAccessManager(this)),
      m_outputDir(QDir(CUTE_MAP_ROOT).filePath("data"))
{
    QDir(CUTE_MAP_ROOT).mkdir("data");
}

void DownloadManager::append(const QUrl &url)
{
    if (m_queue.empty())
        QTimer::singleShot(0, this, SLOT(downloadNext()));

    m_queue.enqueue(url);
}

void DownloadManager::downloadNext()
{
    if (m_queue.isEmpty())
        return;

    QUrl url = m_queue.dequeue();
    m_output.setFileName(m_outputDir.filePath(url.fileName()));
    if (m_output.exists() && m_output.size() > 0) {
        return downloadNext(); // Already downloaded
    }
    if (!m_output.open(QIODevice::WriteOnly)) {
        qWarning("Unable to write file '%s'.", qUtf8Printable(QFileInfo(m_output).fileName()));
        return downloadNext();
    }

    m_reply = m_networkManager->get(QNetworkRequest(url));
    connect(m_reply, SIGNAL(downloadProgress(qint64,qint64)),
            SLOT(progress(qint64,qint64)));
    connect(m_reply, SIGNAL(readyRead()), SLOT(write()));
    connect(m_reply, SIGNAL(finished()), SLOT(finished()));
    qDebug("Downloading '%s'...", qUtf8Printable(QFileInfo(m_output).fileName()));
}

void DownloadManager::progress(qint64 received, qint64 total)
{
    qDebug("%s / %s",
           QString::number(received).toLocal8Bit().constData(),
           QString::number(total).toLocal8Bit().constData());
}

void DownloadManager::write()
{
    m_output.write(m_reply->readAll());
}

void DownloadManager::finished()
{
    m_output.close();

    if (m_reply->error() != QNetworkReply::NoError) {
        qWarning("Error while downloading file '%s': '%s'.",
                 qUtf8Printable(QFileInfo(m_output).fileName()),
                 qUtf8Printable(m_reply->errorString()));
        m_output.remove();
    }
    else {
        qDebug("Download finish '%s'", qUtf8Printable(QFileInfo(m_output).fileName()));
    }

    m_reply->deleteLater();
    downloadNext();
}
