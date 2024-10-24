#include "geometry.hpp"

/// @brief Creates a plane geometry
/// @param x X size of the plane
/// @param z Z size of the plane
/// @return Geometry representing the plane
Geometry create_plane(float x, float z)
{
    float halfX = x / 2.0f;
    float halfZ = z / 2.0f;
    Geometry plane = {};
    plane.vertices = {
        {{-halfX, 0.0f, -halfZ}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
        {{-halfX, 0.0f, halfZ}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{halfX, 0.0f, -halfZ}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{halfX, 0.0f, halfZ}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}}
    };

    plane.indices = {
        0, 1, 2,
        2, 1, 3,
    };

    return plane;
}

/// @brief Creates a box geometry
/// @param width Box's width
/// @param height Box's height
/// @return Geometry representing the box
Geometry create_box(float width, float height)
{
    float halfW = width / 2.0f;
    float halfH = height / 2.0f;
    Geometry box = {};
    box.vertices = {
        // Front
        {{-halfW, halfH, halfW}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
        {{-halfW, -halfH, halfW}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{halfW, halfH, halfW}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{halfW, -halfH, halfW}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},

        // Right
        {{halfW, halfH, halfW}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        {{halfW, -halfH, halfW}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{halfW, halfH, -halfW}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{halfW, -halfH, -halfW}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},

        // Back
        {{halfW, halfH, -halfW}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
        {{halfW, -halfH, -halfW}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
        {{-halfW, halfH, -halfW}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
        {{-halfW, -halfH, -halfW}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},

        // Left
        {{-halfW, halfH, -halfW}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        {{-halfW, -halfH, -halfW}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{-halfW, halfH, halfW}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{-halfW, -halfH, halfW}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},

        // Top
        {{-halfW, halfH, -halfW}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
        {{-halfW, halfH, halfW}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{halfW, halfH, -halfW}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{halfW, halfH, halfW}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},

        // Bottom
        {{-halfW, -halfH, -halfW}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
        {{-halfW, -halfH, halfW}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
        {{halfW, -halfH, -halfW}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
        {{halfW, -halfH, halfW}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}}
    };

    box.indices = {
        0, 1, 2,
        2, 1, 3,

        4, 5, 6,
        6, 5, 7,

        8, 9, 10,
        10, 9, 11,

        12, 13, 14,
        14, 13, 15,

        16, 17, 18,
        18, 17, 19,

        22, 21, 20,
        23, 21, 22
    };

    return box;
}


/// @brief Creates am inverted box geometry
/// @param width Box's width
/// @param height Box's height
/// @return Geometry representing the box
Geometry create_inverted_box(float width, float height)
{
    float halfW = width / 2.0f;
    float halfH = height / 2.0f;
    Geometry box = {};
    box.vertices = {
        // Front
        {{-halfW, halfH, halfW}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
        {{-halfW, -halfH, halfW}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
        {{halfW, halfH, halfW}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
        {{halfW, -halfH, halfW}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},

        // Right
        {{halfW, halfH, halfW}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        {{halfW, -halfH, halfW}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{halfW, halfH, -halfW}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{halfW, -halfH, -halfW}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},

        // Back
        {{halfW, halfH, -halfW}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
        {{halfW, -halfH, -halfW}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{-halfW, halfH, -halfW}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-halfW, -halfH, -halfW}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},

        // Left
        {{-halfW, halfH, -halfW}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        {{-halfW, -halfH, -halfW}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{-halfW, halfH, halfW}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{-halfW, -halfH, halfW}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},

        // Top
        {{-halfW, halfH, -halfW}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
        {{-halfW, halfH, halfW}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
        {{halfW, halfH, -halfW}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
        {{halfW, halfH, halfW}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},

        // Bottom
        {{-halfW, -halfH, -halfW}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
        {{-halfW, -halfH, halfW}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{halfW, -halfH, -halfW}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{halfW, -halfH, halfW}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}}
    };

    box.indices = {
        2, 1, 0,
        3, 1, 2,

        6, 5, 4,
        7, 5, 6,

        10, 9, 8,
        11, 9, 10,

        14, 13, 12,
        15, 13, 14,

        18, 17, 16,
        19, 17, 18,

        20, 21, 22,
        22, 21, 23
    };

    return box;    
}

Geometry create_sphere(unsigned int segments, unsigned int arcs, float radius)
{
    return {};
}

Geometry create_cylinder(unsigned int segments, float radius, float height)
{
    return {};
}