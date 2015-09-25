#ifndef TILERENDERER_H
#define TILERENDERER_H

#include <QThread>

#include <QByteArray>

class QVariant;

class TileRenderer : public QThread
{
    Q_OBJECT

public:
    explicit TileRenderer(QObject *parent = 0);

signals:
    void listening(const QVariant &url);

protected:
    void run() Q_DECL_OVERRIDE;
    virtual QByteArray render(int x, int y, int z) = 0;
};

#endif // TILERENDERER_H
