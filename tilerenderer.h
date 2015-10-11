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
    static inline double longitude(int x, int z);
    static inline double latitude(int y, int z);

    void run() Q_DECL_OVERRIDE;
    virtual QByteArray render(int x, int y, int z) = 0;
};

#include "tilerenderer.hpp"

#endif // TILERENDERER_H
