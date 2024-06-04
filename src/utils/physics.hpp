#pragma once

#include <cgp/cgp.hpp>
#include <vector>
#include "../objects/CelestialBody.hpp"

vec3 compute_gravitional_force(
    const cgp::vec3& position,
    const std::vector<std::unique_ptr<CelestialBody>>& bodies);
