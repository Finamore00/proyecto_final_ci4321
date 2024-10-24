#pragma once
#include <vector>
#include "../../thirdparty/glm/glm.hpp"

#define MATRIX_UBO_BP 0
#define LIGHTS_UBO_BP 1
#define BLOCKS_AMOUNT 2
#define MATRIX_UBO_SIZE 128
#define LIGHTS_UBO_SIZE 36

class Light;
class Transform;
class SceneObject;

struct LightEntity
{
    const Light& light;
    bool active;
};

class RenderingEngine {
private:
    std::vector<LightEntity> m_lights;
    const Transform* m_camera;

    unsigned int m_uboBlocks[BLOCKS_AMOUNT];

    void sync_lights();
    void sync_view_matrix();

public:
    RenderingEngine(float width, float height, float pov);

    void set_main_camera(const Transform* cam);
    void set_projection_matrix(const glm::mat4& projection);
    void register_light(const Light& light);
    void render_tree(const SceneObject& tree, bool first);
};