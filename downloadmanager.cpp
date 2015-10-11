#include "downloadmanager.h"

#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRegularExpression>
#include <QString>
#include <QStringList>
#include <QTimer>

DownloadManager::DownloadManager(QObject *parent)
    : QObject(parent),
      m_networkManager(new QNetworkAccessManager(this)),
      m_outputDir(QDir(CUTEMAP_DIR).filePath("data")),
      m_downloadCount(0),
      m_totalCount(0)
{
}

void DownloadManager::append(const QUrl &url)
{
    ++m_totalCount;
    if (m_queue.empty())
        QTimer::singleShot(0, this, SLOT(downloadNext()));

    m_queue.enqueue(url);
}

void DownloadManager::downloadNext()
{
    ++m_downloadCount;
    if (m_queue.isEmpty()) {
        emit finished();
        return;
    }

    QUrl url = m_queue.dequeue();
    m_output.setFileName(m_outputDir.filePath(url.fileName()));
    if (m_output.exists() && m_output.size() > 0) { // Already downloaded
        return QTimer::singleShot(0, this, SLOT(downloadNext()));
    }
    if (!m_output.open(QIODevice::WriteOnly)) {
        qWarning("Unable to write file '%s'.", qUtf8Printable(m_output.fileName()));
        return QTimer::singleShot(0, this, SLOT(downloadNext()));
    }

    emitProgress("Downloading", 0, -1);
    m_reply = m_networkManager->get(QNetworkRequest(url));
    connect(m_reply, SIGNAL(downloadProgress(qint64,qint64)),
            SLOT(downloadProgress(qint64,qint64)));
    connect(m_reply, SIGNAL(readyRead()), SLOT(write()));
    connect(m_reply, SIGNAL(finished()), SLOT(downloaded()));
}

void DownloadManager::downloadProgress(qint64 received, qint64 total)
{
    emitProgress("Downloading", received, total);
}

void DownloadManager::write()
{
    m_output.write(m_reply->readAll());
}

void DownloadManager::downloaded()
{
    m_output.close();
    m_reply->deleteLater();

    if (m_reply->error() != QNetworkReply::NoError) {
        qWarning("Error while downloading file '%s': '%s'.",
                 qUtf8Printable(QFileInfo(m_output).fileName()),
                 qUtf8Printable(m_reply->errorString()));
        m_output.remove();

        return QTimer::singleShot(0, this, SLOT(downloadNext()));
    }
    emitProgress("Downloaded", 1, 1);

    QString extension = QFileInfo(m_output).completeSuffix();
    if (!(QStringList() << "osm.bz2" << "shp.zip").contains(extension))
        return QTimer::singleShot(0, this, SLOT(downloadNext()));

    emitProgress("Extracting", 0, -1);
    QString extractPath = m_outputDir.path();
    if (extension == "shp.zip")
        extractPath = m_output.fileName().remove(QRegularExpression(".zip$"));
    m_extractionProcess = new QProcess(this);
    connect(m_extractionProcess, SIGNAL(finished(int)),
            SLOT(extracted(int)));
    connect(m_extractionProcess, SIGNAL(error(QProcess::ProcessError)),
            SLOT(extractionError(QProcess::ProcessError)));
    // m_extractProcess->setProcessChannelMode(QProcess::ForwardedChannels);
    m_extractionProcess->start(QDir(P7ZIP_DIR).filePath("bin/7z"),
                               QStringList() << "x"
                                             << QString("-o%1").arg(extractPath)
                                             << m_output.fileName());
}

void DownloadManager::extracted(int exitCode)
{
    if (exitCode == QProcess::NormalExit) {
        emitProgress("Extracted", 1, 1);
    } else {
        qWarning("Error while extracting file '%s': exit code %d.",
                 qUtf8Printable(QFileInfo(m_output).fileName()),
                 exitCode);
        m_output.remove();
    }

    m_extractionProcess->deleteLater();
    return QTimer::singleShot(0, this, SLOT(downloadNext()));
}

void DownloadManager::extractionError(QProcess::ProcessError /*error*/)
{
    qWarning("Error while extracting file '%s': %s.",
             qUtf8Printable(QFileInfo(m_output).fileName()),
             qUtf8Printable(m_extractionProcess->errorString()));
    m_output.remove();

    m_extractionProcess->deleteLater();
    return QTimer::singleShot(0, this, SLOT(downloadNext()));
}

void DownloadManager::emitProgress(const QString &text, qint64 processed, qint64 total)
{
    emit progress(QString("%1 %2 (%3/%4)").arg(text)
                                          .arg(QFileInfo(m_output).fileName())
                                          .arg(m_downloadCount)
                                          .arg(m_totalCount),
                  processed,
                  total);
}
