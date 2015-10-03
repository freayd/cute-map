#include "tilerenderer.h"

#include <QDateTime>
#include <QHostAddress>
#include <QMimeDatabase>
#include <QMimeType>
#include <QRegularExpression>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUrl>
#include <QVariant>

TileRenderer::TileRenderer(QObject *parent)
    : QThread(parent)
{
}

void TileRenderer::run()
{
    QTcpServer server;
    if (!server.listen(QHostAddress::LocalHost)) {
        qWarning("Unable to start the HTTP server: %s",
                 qUtf8Printable(server.errorString()));
    }

    QUrl url;
    url.setScheme("http");
    url.setHost(server.serverAddress().toString());
    url.setPort(server.serverPort());
    url.setPath("/");
    emit listening(url);

    while(server.hasPendingConnections() || server.waitForNewConnection(-1)) {
        QTcpSocket *socket = server.nextPendingConnection();
        socket->waitForReadyRead();
        QByteArray request = socket->readAll();
        QString requestUri = request.left(request.indexOf("\r\n")).split(' ').at(1);

        QRegularExpression tileExpression("^/(\\d+)/(\\d+)/(\\d+)\\.png$");
        QRegularExpressionMatch tileMatch = tileExpression.match(requestUri);
        if (tileMatch.hasMatch()) {
            int x = tileMatch.captured(2).toInt();
            int y = tileMatch.captured(3).toInt();
            int z = tileMatch.captured(1).toInt();
            QString date = QDateTime::currentDateTimeUtc().toString("ddd, dd MMM yyyy hh:mm:ss 'GMT'");

            QByteArray responseBody = render(x, y, z);
            QMimeType mime = QMimeDatabase().mimeTypeForData(responseBody);
            if (mime.inherits("image/png")) {
                QByteArray response("HTTP/1.0 200 OK\r\n");
                response.append(QString("Date: %1\r\n").arg(date));
                response.append(QString("Content-Type: %1\r\n").arg(mime.name()));
                response.append(QString("Content-Length: %1\r\n").arg(responseBody.size()));
                response.append("\r\n");
                response.append(responseBody);
                socket->write(response);
            } else if (mime.inherits("text/plain")) {
                qWarning("Invalid image returned for %s (mime type: %s):\n%s",
                         qUtf8Printable(requestUri),
                         qUtf8Printable(mime.name()),
                         qUtf8Printable(responseBody));
            } else {
                qWarning("Invalid mime type returned for %s: %s",
                         qUtf8Printable(requestUri),
                         qUtf8Printable(mime.name()));
            }
        } else {
            qWarning("Invalid uri: %s", qUtf8Printable(requestUri));
        }

        socket->disconnectFromHost();
        if (socket->state() != QAbstractSocket::UnconnectedState)
            socket->waitForDisconnected();
        delete socket;
    }
}
