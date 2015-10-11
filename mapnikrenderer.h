#ifndef MAPNIKRENDERER_H
#define MAPNIKRENDERER_H

#include "tilerenderer.h"

#include <QString>

class MapnikRenderer : public TileRenderer
{
    Q_OBJECT

public:
    explicit MapnikRenderer(const QString &mapPath, QObject *parent = 0);

protected:
    QByteArray render(int x, int y, int z) Q_DECL_OVERRIDE;

private:
    QString m_mapPath;
};

#endif // MAPNIKRENDERER_H
