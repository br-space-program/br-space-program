#include "tree.hpp"

using namespace cgp;

mesh create_cylinder_mesh(float radius, float height)
{
    mesh m;
    // To do: fill this mesh ...
    // ...
    // To add a position:
    //   m.position.push_back(vec3{x,y,z})
    // Or in pre-allocating the buffer:
    //   m.position.resize(maximalSize);
    //   m.position[index] = vec3{x,y,z}; (with 0<= index < maximalSize)
    //
    // Similar with the triangle connectivity:
    //  m.connectivity.push_back(uint3{index_1, index_2, index_3});

    const int nbr_vertex = 10; // Nbr vertex per circle

    for (int i = 0; i < nbr_vertex; i++)
    {
        vec3 bas = vec3(radius * cos(2 * Pi * i / nbr_vertex), radius * sin(2 * Pi * i / nbr_vertex), 0);
        vec3 haut = bas + vec3(0.0f, 0.0f, height);

        m.position.push_back(bas);
        m.position.push_back(haut);

        if (i < nbr_vertex)
        {
            m.connectivity.push_back(uint3{2 * i,
                                           (2 * i + 1) % (2 * nbr_vertex),
                                           (2 * i + 2) % (2 * nbr_vertex)});
            m.connectivity.push_back(uint3{2 * i + 1,
                                           (2 * i + 2) % (2 * nbr_vertex),
                                           (2 * i + 3) % (2 * nbr_vertex)});
        }
    }

    // Need to call fill_empty_field() before returning the mesh
    //  this function fill all empty buffer with default values (ex. normals, colors, etc).
    m.fill_empty_field();

    return m;
}

mesh create_cone_mesh(float radius, float height, float z_offset)
{
    mesh m;

    const int nbr_vertex = 5;
    // Sommet du haut
    m.position.push_back(vec3(0.0f, 0.0f, height + z_offset));

    for (int i = 0; i < nbr_vertex; i++)
    {
        vec3 vertex = vec3(radius * cos(2 * Pi * i / nbr_vertex), radius * sin(2 * Pi * i / nbr_vertex), z_offset);

        m.position.push_back(vertex);
        m.connectivity.push_back(uint3{0,
                                       1 + i,
                                       1 + (i + 1) % nbr_vertex});
    }

    m.fill_empty_field();
    return m;
}

mesh create_tree()
{
    float h = 0.7f; // trunk height
    float r = 0.1f; // trunk radius

    // Create a brown trunk
    mesh trunk = create_cylinder_mesh(r, h);
    trunk.color.fill({0.4f, 0.3f, 0.3f});

    // Create a green foliage from 3 cones
    mesh foliage = create_cone_mesh(4 * r, 6 * r, 0.0f);      // base-cone
    foliage.push_back(create_cone_mesh(4 * r, 6 * r, 2 * r)); // middle-cone
    foliage.push_back(create_cone_mesh(4 * r, 6 * r, 4 * r)); // top-cone
    foliage.translate({0, 0, h});                             // place foliage at the top of the trunk
    foliage.color.fill({0.4f, 0.6f, 0.3f});

    // The tree is composed of the trunk and the foliage
    mesh tree = trunk;
    tree.push_back(foliage);

    return tree;
}