#ifndef TILERENDERER_HPP
#define TILERENDERER_HPP

#include <cmath>

double TileRenderer::longitude(int x, int z)
{
    return x / exp2(z) * 360.0 - 180;
}

double TileRenderer::latitude(int y, int z)
{
    double n = M_PI - 2.0 * M_PI * y / exp2(z);
    return 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
}

#endif // TILERENDERER_HPP
