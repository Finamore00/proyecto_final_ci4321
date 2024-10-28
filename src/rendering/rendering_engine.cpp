#include "rendering_engine.hpp"
#include "lights.hpp"

#include <iostream>

#include "../../thirdparty/glm/gtc/matrix_transform.hpp"
#include "../../thirdparty/glad/include/glad/glad.h"

#include "../scene_graph/transform.hpp"
#include "../mesh/mesh.hpp"


RenderingEngine::RenderingEngine(float width, float height, float pov)
{

    // UBO blocks set up
    glGenBuffers(BLOCKS_AMOUNT, m_uboBlocks);
    // Matrix block
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboBlocks[MATRIX_UBO_BP]);
    glBufferData(GL_UNIFORM_BUFFER, MATRIX_UBO_SIZE, NULL, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, MATRIX_UBO_BP, m_uboBlocks[MATRIX_UBO_BP]); 

    // Light block
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboBlocks[LIGHTS_UBO_BP]);
    glBufferData(GL_UNIFORM_BUFFER, LIGHTS_UBO_SIZE, NULL, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, LIGHTS_UBO_BP, m_uboBlocks[LIGHTS_UBO_BP]); 

    set_projection_matrix(glm::perspective(glm::radians(pov), width/height, 0.1f, 1000.0f));
}

/// @brief Syncs the UBO Lights block with the scene lights
void RenderingEngine::sync_lights()
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboBlocks[LIGHTS_UBO_BP]);
    for (unsigned int l = 0; l < m_lights.size(); ++l)
    {
        const Light& lo = m_lights[l].light;
        float pow = lo.intensity * (lo.enabled ? 1.0f : 0.0f);
        glm::vec3 lp = lo.transform.get_world_position();

        glBufferSubData(GL_UNIFORM_BUFFER, 0, 16, &lp); 
        glBufferSubData(GL_UNIFORM_BUFFER, 16, 16, &lo.color);
        glBufferSubData(GL_UNIFORM_BUFFER, 32, 4, &pow); 
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}

/// @brief Syncs the view matrix in the UBO Matrices block with the camera
void RenderingEngine::sync_view_matrix()
{
    glm::mat4 view = glm::lookAt(
        m_camera->get_world_position(), 
        m_camera->get_world_position() + m_camera->get_front_vector(), 
        m_camera->get_up_vector()
    );

    glBindBuffer(GL_UNIFORM_BUFFER, m_uboBlocks[MATRIX_UBO_BP]);
    glBufferSubData(GL_UNIFORM_BUFFER, 64, 64, &view); 
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

/// @brief Sets the main camera of the renderer
/// @param cam Target main camera
void RenderingEngine::set_main_camera(const Transform* cam)
{
    m_camera = cam;
    sync_view_matrix();
}

/// @brief Syncs the UBO Matrices block with a new projection matrix
/// @param projection Target projection matrix
void RenderingEngine::set_projection_matrix(const glm::mat4& projection)
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboBlocks[MATRIX_UBO_BP]);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, &projection); 
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

/// @brief Registers a light in the rendering engine
/// @param light Light to register
void RenderingEngine::register_light(const Light& light)
{
    m_lights.push_back({light, light.enabled});
}

/// @brief Renders a scene object tree
/// @param tree Root of the tree
/// @param first Indicates if this is the first render of the frame
void RenderingEngine::render_tree(const SceneObject& tree, bool first)
{
    if (first)
    {
        sync_view_matrix();
        sync_lights();
    }

    if (tree.mesh != nullptr && first)
    {   
        tree.mesh->shader->use();
        tree.mesh->shader->set_mat4f("model", tree.transform.get_model_matrix());
        tree.mesh->draw();
    }

    for (auto &&c : tree.transform.get_children())
    {
        const SceneObject& co = c->get_scene_object();
        if (!co.enabled)
            continue;;

        if (co.mesh != nullptr)
        {   
            co.mesh->shader->use();
            co.mesh->shader->set_mat4f("model", c->get_model_matrix());
            co.mesh->draw();
        }

        render_tree(co, false);
    }
}