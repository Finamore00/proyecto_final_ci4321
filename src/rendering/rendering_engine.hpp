#pragma once
#include "../../thirdparty/glad/include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>

#include "shadow_engine.hpp"
#include "light_component.hpp"

#include "../../thirdparty/glm/glm.hpp"

#include "../textures/texture.hpp"
#include "../gl_utils/shader.hpp"
#include "../mesh/mesh.hpp"

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
    static RenderingEngine *g_instance;

    GLFWwindow& m_window;
    float m_width, m_height, m_pov;
    
    gl_utils::shader_program m_screen_prog;
    gl_utils::shader_program m_skybox_prog;
    glm::mat4 m_ui_proj;

    Mesh m_skybox_mesh;
    const Transform* m_camera;
    const SceneObject* m_scene_root = nullptr;
    SceneObject* m_ui_root = nullptr;

    unsigned int m_colTx[2] = {0, 0};
    unsigned int m_fbs[2] = {0, 0};
    unsigned int m_rbfs[2] = {0, 0};

    unsigned int m_uboBlocks[BLOCKS_AMOUNT];
    unsigned int m_cqvbo = 0, m_cqvao = 0;

    ShadowEngine m_shadows;
    std::vector<LightEntity> m_lights;
    
    void sync_lights();
    void sync_view_matrix();
    void prepare_container_quad();
    void prepare_framebuffers();
    void render_screen_texture(unsigned int tx);
    void render_tree(const SceneObject& tree, bool first);
    void render_ui(SceneObject& tree, bool first);

public:
    RenderingEngine(GLFWwindow& window, float width, float height, float pov);

    static RenderingEngine* get_instance();

    void set_resolution(float width, float height);
    void set_ui_resolution(float width, float height);
    void set_pov(float pov);
    void set_main_camera(const Transform* cam);
    void set_projection_matrix(const glm::mat4& projection);
    void set_scene_root(const SceneObject* root);
    void set_ui_root(SceneObject* root);
    void set_skybox_texture(shared_ptr<Texture> texture);
    void register_light(const Light& light);
    void register_light(const LightComponent& light);
    void render();
};