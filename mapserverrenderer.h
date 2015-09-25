#ifndef MAPSERVERRENDERER_H
#define MAPSERVERRENDERER_H

#include "tilerenderer.h"

class QString;

class MapServerRenderer : public TileRenderer
{
    Q_OBJECT

public:
    explicit MapServerRenderer(const QString &mapPath, QObject *parent = 0);

protected:
    QByteArray render(int x, int y, int z) Q_DECL_OVERRIDE;
};

#endif // MAPSERVERRENDERER_H
