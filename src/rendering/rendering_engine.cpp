#include "rendering_engine.hpp"
#include "lights.hpp"

#include <iostream>

#include "../../thirdparty/glm/gtc/matrix_transform.hpp"

#include "../scene_graph/transform.hpp"
#include "../mesh/mesh.hpp"

#define SCENE_FB 0
#define UI_FB 1

#define SCENE_RBFS 0
#define UI_RBFS 1

#define SCENE_COL 0
#define UI_COL 1

#define SCENE_ATT 0
#define UI_ATT 1

const float CONTAINER_QUAD_VTX[] = {   
        // pos         // uvs
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
};


RenderingEngine::RenderingEngine(GLFWwindow& window, float width, float height, float pov)
    :   m_window(window), m_width(width), m_height(height), m_pov(pov), 
        m_screen_prog("../shader_files/screen.fb.vert", "../shader_files/screen.fb.frag"),
        m_skybox_prog("../shader_files/skybox.vert", "../shader_files/skybox.frag")
{
    m_skybox_mesh.initialize_geometry(create_inverted_box(1000.0f, 1000.0f));
    m_skybox_mesh.set_shader(m_skybox_prog);

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

    set_projection_matrix(glm::perspective(glm::radians(pov), width/height, 0.1f, 2000.0f));
    prepare_framebuffers();
    prepare_container_quad();
}

#pragma region CONFS

void RenderingEngine::prepare_container_quad()
{   if (m_cqvbo != 0)
    {
        glDeleteBuffers(1, &m_cqvbo);
        glDeleteVertexArrays(1, &m_cqvao);
    }

    glGenBuffers(1, &m_cqvbo);
    glGenVertexArrays(1, &m_cqvao);
    
    glBindVertexArray(m_cqvao);
    glBindBuffer(GL_ARRAY_BUFFER, m_cqvbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CONTAINER_QUAD_VTX), CONTAINER_QUAD_VTX, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);
}

void RenderingEngine::prepare_framebuffers()
{
    if (m_colTx[0] != 0)
    {
        glDeleteTextures(2, m_colTx);
        glDeleteFramebuffers(2, m_fbs);
        glDeleteRenderbuffers(2, m_rbfs);
    }

    glGenFramebuffers(2, m_fbs);
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbs[SCENE_FB]);
    glGenTextures(1, &m_colTx[SCENE_COL]);
    glBindTexture(GL_TEXTURE_2D, m_colTx[SCENE_COL]);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colTx[SCENE_COL], 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenRenderbuffers(1, &m_rbfs[SCENE_ATT]);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbfs[SCENE_ATT]);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbfs[SCENE_ATT]);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbs[UI_FB]);
    glGenTextures(1, &m_colTx[UI_COL]);
    glBindTexture(GL_TEXTURE_2D, m_colTx[UI_COL]);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colTx[UI_COL], 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenRenderbuffers(1, &m_rbfs[UI_ATT]);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbfs[UI_ATT]);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH, m_width, m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rbfs[UI_ATT]);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderingEngine::set_resolution(float width, float height)
{
    m_width = width;
    m_height = height;
    set_projection_matrix(glm::perspective(glm::radians(m_pov), width/height, 0.1f, 1000.0f));
    prepare_framebuffers();
}

void RenderingEngine::set_pov(float pov)
{
    set_projection_matrix(glm::perspective(glm::radians(m_pov), m_width/m_height, 0.1f, 1000.0f));
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

void RenderingEngine::set_scene_root(const SceneObject* root)
{
    m_scene_root = root;
}

void RenderingEngine::set_ui_root(const SceneObject* root)
{
    m_ui_root = root;
}

void RenderingEngine::set_skybox_texture(shared_ptr<Texture> texture)
{
    m_skybox_mesh.shaderMaterial.albedo = texture;
}

#pragma endregion

#pragma region SYNCS
/// @brief Syncs the UBO Lights block with the scene lights
void RenderingEngine::sync_lights()
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboBlocks[LIGHTS_UBO_BP]);
    for (unsigned int l = 0; l < m_lights.size(); ++l)
    {
        const Light& lo = m_lights[l].light;
        float pow = lo.intensity * (lo.active ? 1.0f : 0.0f);
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
#pragma endregion

/// @brief Registers a light in the rendering engine
/// @param light Light to register
void RenderingEngine::register_light(const Light& light)
{
    m_lights.push_back({light, light.active});
}

#pragma region RENDER
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
        if (!co.active)
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

void RenderingEngine::render_screen_texture(unsigned int tx)
{
    glUseProgram(m_screen_prog.program_id);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tx);
    
    glBindVertexArray(m_cqvao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void RenderingEngine::render()
{    
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbs[SCENE_FB]);
    glClearColor(0.136f, 0.136f, 0.136f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    glDepthMask(GL_FALSE);
    m_skybox_mesh.shader->use();
    m_skybox_mesh.draw();

    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    render_tree(*m_scene_root, true);

    //glBindFramebuffer(GL_FRAMEBUFFER, m_fbs[UI_FB]);
    //glClearColor(0.106f, 0.118f, 0.169f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //render_ui();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    render_screen_texture(m_colTx[SCENE_COL]);
    //render_screen_texture(m_colTx[UI_COL]);
    glfwSwapBuffers(&m_window);
}
#pragma endregion