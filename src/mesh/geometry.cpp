#include "geometry.hpp"
#include <iostream>

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
Geometry create_box(float x, float y, float z)
{
    float halfX = x / 2.0f;
    float halfY = y / 2.0f;
    float halfZ = z / 2.0f;
    Geometry box = {};
    box.vertices = {
        // Front
        {{-halfX, halfY, halfZ}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
        {{-halfX, -halfY, halfZ}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{halfX, halfY, halfZ}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{halfX, -halfY, halfZ}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},

        // Right
        {{halfX, halfY, halfZ}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        {{halfX, -halfY, halfZ}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{halfX, halfY, -halfZ}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{halfX, -halfY, -halfZ}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},

        // Back
        {{halfX, halfY, -halfZ}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
        {{halfX, -halfY, -halfZ}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
        {{-halfX, halfY, -halfZ}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
        {{-halfX, -halfY, -halfZ}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},

        // Left
        {{-halfX, halfY, -halfZ}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        {{-halfX, -halfY, -halfZ}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{-halfX, halfY, halfZ}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{-halfX, -halfY, halfZ}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},

        // Top
        {{-halfX, halfY, -halfZ}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
        {{-halfX, halfY, halfZ}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{halfX, halfY, -halfZ}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{halfX, halfY, halfZ}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},

        // Bottom
        {{-halfX, -halfY, -halfZ}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
        {{-halfX, -halfY, halfZ}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
        {{halfX, -halfY, -halfZ}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
        {{halfX, -halfY, halfZ}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}}
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

/**
 * @brief Creates and returns a sphere geometry
 * @param sectors Sector count for the uv sphere. Must be greater than 0
 * @param stacks Stack count for the uv sphere. Must be greater than 0
 * @param radius Radius of the sphere
 * @return Geometry representing the sphere shaper
 */
Geometry create_sphere(unsigned int sectors, unsigned int stacks, float radius)
{
    Geometry sphere = {};
    sphere.vertices = {};
    sphere.indices = {};


    const float sector_step = (2.0f * PI) / static_cast<float>(sectors);
    const float stack_step = PI / static_cast<float>(stacks);
    const float rad_invert = 1.0f / radius;


    for (unsigned int i = 0; i <= stacks; ++i) {
        const float stack_angle = (PI / 2.0f) - (i * stack_step);
        const float xz_plane_coord = radius * cosf(stack_angle);
        const float y_coord = radius * sinf(stack_angle);

        //Aux variables for indices
        int ind_aux1 = i * (sectors + 1);
        int ind_aux2 = ind_aux1 + sectors + 1;

        for (unsigned int j = 0; j <= sectors; ++j) {
            const float sector_angle = j * sector_step;
            const float z_coord = xz_plane_coord * cosf(sector_angle);
            const float x_coord = xz_plane_coord * sinf(sector_angle);

            const glm::vec3 coords = {x_coord, y_coord, z_coord};
            const glm::vec3 normals = glm::normalize(coords);
            const glm::vec2 tex_pos = {static_cast<float>(j) / sectors, static_cast<float>(i) / stacks};

            sphere.vertices.push_back({coords, normals, tex_pos});

            //Adding indices
            if (j < sectors) {
                if (i != 0) {
                    sphere.indices.push_back(ind_aux1);
                    sphere.indices.push_back(ind_aux2);
                    sphere.indices.push_back(ind_aux1 + 1);
                }

                if (i != stacks - 1) {
                    sphere.indices.push_back(ind_aux1 + 1);
                    sphere.indices.push_back(ind_aux2);
                    sphere.indices.push_back(ind_aux2 + 1);
                }

                ind_aux1++;
                ind_aux2++;
            }
        }
    }

    return sphere;
}

/**
 * @brief Creates and returns a cylinder geometry
 * @param sectors Sector count for the cylinder circunference. Must be greater than 0
 * @param radius Radius of the sphere circunference
 * @param height Cylinder height
 * @return A geometry object representing the cylinder.
 */
Geometry create_cylinder(unsigned int sectors, float radius, float height)
{
    Geometry cylinder = {};
    cylinder.vertices = {};
    cylinder.indices = {};

    const float sector_step = (2.0f * PI) / static_cast<float>(sectors);
    const float rad_inv = 1.0f / radius;
    const float hf_height = height / 2.0f;
    const float hf_height_inv = 1.0f / hf_height;

    //Points at the center of the top and bottom circumferences
    cylinder.vertices.push_back({glm::vec3(0.0f, hf_height, 0.0f),
                                 glm::vec3(0.0f, 1.0f, 0.0f),
                                 glm::vec2(0.5f, 0.5f)}); //See tex coords later

    cylinder.vertices.push_back({glm::vec3(0.0f, -hf_height, 0.0f),
                                 glm::vec3(0.0f, -1.0f, 0.0f),
                                 glm::vec2(0.5f, 0.5f)});

    int j = 0;
    // Push all remaining points and indices
    for (unsigned int i = 0; i <= sectors; i++) {
        const float sector_angle = i * sector_step;
        const float x_coord = radius * cosf(sector_angle);
        const float z_coord = radius * sinf(sector_angle);

        const glm::vec3 coords_top = {x_coord, hf_height, z_coord};
        const glm::vec3 normals_top = {x_coord * rad_inv, hf_height * hf_height_inv, 0.0f};
        const glm::vec2 uvs_top = {(float)i / sectors, 1.0f};

        const glm::vec3 coords_bottom = {x_coord, -hf_height, z_coord};
        const glm::vec3 normals_bottom = {x_coord * rad_inv, hf_height * hf_height_inv, 0.0f};
        const glm::vec2 uvs_bottom = {(float)i / sectors, 0.0f};

        cylinder.vertices.push_back({coords_top, normals_top, uvs_top});
        cylinder.vertices.push_back({coords_bottom, normals_bottom, uvs_bottom});

        //Push indices for top triangle
        cylinder.indices.push_back(0);
        cylinder.indices.push_back(j + 4);
        cylinder.indices.push_back(j + 2);

        //Push indices for bottom triangle
        cylinder.indices.push_back(1);
        cylinder.indices.push_back(j + 3);
        cylinder.indices.push_back(j + 5);

        //Push indices for side plane
        cylinder.indices.push_back(j + 2);
        cylinder.indices.push_back(j + 4);
        cylinder.indices.push_back(j + 3);
        cylinder.indices.push_back(j + 3);
        cylinder.indices.push_back(j + 4);
        cylinder.indices.push_back(j + 5);

        j += 2;
    }
    return cylinder;
}