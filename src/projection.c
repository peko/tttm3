#include <math.h>

#include "projection.h"

/*
lat,lng -> laea x,y
a = sin(lat0);
b = cos(lat0);
c = sin(lat);
d = cos(lat);
e = sin(lng-lng0);
f = cos(lng-lng0);
k = sqrt(2.0/(1.0+a*c+b*d*f));
x = k*d*e;
y = k*(b*c-a*d*f);
*/

void wgs2laea(
    point_t  center,
    point_t* points,
    point_t* result,
    size_t count) {

    double a, b, c, d, e, f, k;

    a = sin(center.y);
    b = cos(center.y);
    for(size_t i=0; i<count; i++) {

        c = sin(points[i].y);
        d = cos(points[i].y);
        e = sin(points[i].x-center.x);
        f = cos(points[i].x-center.x);
        k = sqrt(2.0/(1.0+a*c+b*d*f));

        result[i].x = k*d*e;
        result[i].y = k*(b*c-a*d*f);
    }
}

/*
laea x,y -> lat,lng
a = sin(lat0);
b = cos(lat0);
r = sqrt(x*x+y*y);
c = 2.0/sin(r/2.0);
g = sin(c);
h = cos(c);
lat = 1.0 / sin(h*a+y*g*b/r)
lng = lng0 + 1.0/tan(x*g/(r*b*h - y*a*g))
*/

void laea2wgs(
    point_t  center,
    point_t* points,
    point_t* result,
    size_t count) {

    double a, b, r, c, g, h, x, y;
    a = sin(center.y);
    b = cos(center.y);

    for(size_t i=0; i<count; i++) {
        x = points[i].x;
        y = points[i].y;
        r = sqrt(x*x+y*y);
        c = 2.0/sin(r/2.0);
        g = sin(c);
        h = cos(c);
        result[i].y = 1.0 / sin(h*a+y*g*b/r);
        result[i].x = center.x + 1.0/tan(x*g/(r*b*h - y*a*g));
    }
}