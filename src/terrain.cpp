
#include "terrain.hpp"
#include "scene.hpp"

using namespace cgp;

float gauss(vec2 x, float amplitude, vec2 esperance, float ecart_type)
{

    float d = norm(x - esperance) / ecart_type;
    return amplitude * std::exp(-d * d);
}

// Evaluate 3D position of the terrain for any (x,y)
float evaluate_terrain_height(float x, float y, float terrain_length)
{

    const float u = x / terrain_length;
    const float v = y / terrain_length;

    // Compute the Perlin noise
    const float persistency = 0.543f;
    const float freq = 1.8f;
    const int octave = 8;
    const float height = 3.0f;
    float const noise = noise_perlin({u, v}, octave, persistency, freq);

    // use the noise as height value
    return height * noise;
}

mesh create_terrain_mesh(int N, float terrain_length)
{

    mesh terrain; // temporary terrain storage (CPU only)
    terrain.position.resize(N * N);
    terrain.uv.resize(N * N);

    // Fill terrain geometry
    for (int ku = 0; ku < N; ++ku)
    {
        for (int kv = 0; kv < N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            float u = ku / (N - 1.0f);
            float v = kv / (N - 1.0f);

            // Compute the real coordinates (x,y) of the terrain in [-terrain_length/2, +terrain_length/2]
            float x = (u - 0.5f) * terrain_length;
            float y = (v - 0.5f) * terrain_length;

            // Compute the surface height function at the given sampled coordinate
            float z = evaluate_terrain_height(x, y, terrain_length);

            // Store vertex coordinates
            terrain.position[kv + N * ku] = {x, y, z};
            terrain.uv[kv + N * ku] = {u * 10.0, v * 10.0};
        }
    }

    // Generate triangle organization
    //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
    for (int ku = 0; ku < N - 1; ++ku)
    {
        for (int kv = 0; kv < N - 1; ++kv)
        {
            unsigned int idx = kv + N * ku; // current vertex offset

            uint3 triangle_1 = {idx, idx + 1 + N, idx + 1};
            uint3 triangle_2 = {idx, idx + N, idx + 1 + N};

            terrain.connectivity.push_back(triangle_1);
            terrain.connectivity.push_back(triangle_2);
        }
    }

    // need to call this function to fill the other buffer with default values (normal, color, etc)
    terrain.fill_empty_field();

    return terrain;
}

float rand_interval(float a, float b)
{

    return a + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (b - a)));
}

bool tree_in_other_tree(float x, float y, float safe_width, std::vector<cgp::vec3> &trees)
{
    for (int i = 0; i < trees.size(); i++)
    {
        if (pow(x - trees[i].x, 2) + pow(y - trees[i].y, 2) < pow(safe_width, 2))
        {
            return true;
        }
    }
    return false;
}

std::vector<cgp::vec3> generate_positions_on_terrain(int N, float terrain_length, float safe_width)
{
    std::vector<vec3> res;

    for (int i = 0; i < N; i++)
    {
        float x;
        float y;

        do
        {
            x = rand_interval(-terrain_length / 2, terrain_length / 2);
            y = rand_interval(-terrain_length / 2, terrain_length / 2);
        } while (tree_in_other_tree(x, y, safe_width, res));

        res.push_back(vec3(x, y, evaluate_terrain_height(x, y, terrain_length)));
    }

    return res;
}
