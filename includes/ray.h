#pragma once

#include "vec3.h"

class ray {
public:
    ray() {}

    // constructor with point and direction
    ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

    // get origin point
    point3 origin() const { return orig; }

    // get direction as a 3d vector
    vec3 direction() const { return dir; }

    // current position at time t
    point3 at(double t) const {
        return orig + t * dir;
    }

private:
    // origin point
    point3 orig;

    // direction as 3d vector
    vec3 dir;
};