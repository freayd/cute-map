#include "mapnikrenderer.h"

#include <mapnik/agg_renderer.hpp>
#include <mapnik/box2d.hpp>
#include <mapnik/config_error.hpp>
#include <mapnik/datasource_cache.hpp>
#include <mapnik/image.hpp>
#include <mapnik/image_util.hpp>
#include <mapnik/layer.hpp>
#include <mapnik/load_map.hpp>
#include <mapnik/map.hpp>
#include <mapnik/params.hpp>

#include <exception>

#include <QDir>
#include <QStringList>

MapnikRenderer::MapnikRenderer(const QString &mapPath, QObject *parent)
    : TileRenderer(parent),
      m_mapPath(mapPath)
{
    QString pluginFile = QDir(MAPNIK_DIR).filePath("lib/mapnik/input/shape.input");
    mapnik::datasource_cache::instance().register_datasource(pluginFile.toLocal8Bit().constData());
}

QByteArray MapnikRenderer::render(int x, int y, int z)
{
    QString xmlFile = QDir(CUTEMAP_DIR).filePath("data/mapnik.xml");

    mapnik::Map map(256, 256);
    mapnik::load_map(map, xmlFile.toLocal8Bit().constData());

    QStringList layers;
    layers << "buildings" << "roads";
    for (int l = 0; l < layers.size(); ++l) {
        mapnik::parameters params;
        params["type"] = "shape";
        params["file"] = QDir(m_mapPath).filePath(layers.at(l) + ".shp").toLocal8Bit().constData();

        mapnik::layer layer(layers.at(l).toLocal8Bit().constData());
        layer.set_datasource(mapnik::datasource_cache::instance().create(params));
        layer.add_style(layer.name());
        map.add_layer(layer);
    }

    map.set_aspect_fix_mode(mapnik::Map::RESPECT);
    map.zoom_to_box(mapnik::box2d<double>(longitude(x, z),
                                          latitude(y + 1, z),
                                          longitude(x + 1, z),
                                          latitude(y, z)));

    mapnik::image_rgba8 buffer(map.width(), map.height());
    mapnik::agg_renderer<mapnik::image_rgba8> renderer(map, buffer);

    try {
        renderer.apply();
        return QByteArray::fromStdString(mapnik::save_to_string(buffer, "png"));
    } catch (mapnik::config_error & ex) {
        qWarning("Mapnik configuration error: %s", ex.what());
        return QByteArray();
    } catch (const std::exception & ex) {
        qWarning("Mapnik exception: %s", ex.what());
        return QByteArray();
    } catch (...) {
        qWarning("Unknown Mapnik exception!");
        return QByteArray();
    }
}
