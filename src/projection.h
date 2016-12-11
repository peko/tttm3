#include "types.h"

// http://mathworld.wolfram.com/LambertAzimuthalEqual-AreaProjection.html
// lat,lng -> laea x,y
void wgs2laea(point_t center, point_t* points, point_t* result, size_t count);
// laea x,y -> lat,lng
void laea2wgs(point_t center, point_t* points, point_t* result, size_t count);