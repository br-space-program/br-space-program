#pragma once

#include <cgp/cgp.hpp>

using cgp::mesh;
using cgp::vec3;

mesh create_sphere_mesh(float R, vec3 center, int Nu, int Nv);
