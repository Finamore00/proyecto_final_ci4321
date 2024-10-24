#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "thirdparty/glm/gtc/matrix_transform.hpp"
#include "thirdparty/glm/gtc/type_ptr.hpp"
#include "thirdparty/stb/stb_image.h"
#include "thirdparty/glm/glm.hpp"

#include "src/collisions/physic_engine.hpp"
#include "src/scene_graph/transform.hpp"
#include "src/textures/texture.hpp"
#include "src/mesh/geometry.hpp"
#include "src/mesh/mesh.hpp"
#include "src/gl_utils/shader.h"
#include "src/game/bullet.hpp"


// Window resize handler
void windowResizeCallback(GLFWwindow* window, int width, int height);
// Input processing
void processInput(GLFWwindow* window);


glm::vec3 input(0.0f);
glm::vec2 rotInput(0.0f);
glm::mat4 projection, view;
PhysicEngine physicsEngine;


void do_physics();
void logic(const SceneObject& root);
void render_tree(const SceneObject& root);
void draw_skybox(const Mesh& mesh, const Texture& text);

int main()
{
    // Window initializantion
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Triangle", NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, windowResizeCallback);

    // Glad initialization for buffer~memory handling.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        return -1;
    }

    // Open GL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    gl_utils::shader_program basicShader = gl_utils::shader_program(
        "../shader_files/basic.vert",
        "../shader_files/basic.frag"
    );
    gl_utils::shader_program skyboxShader = gl_utils::shader_program(
        "../shader_files/skybox.vert",
        "../shader_files/skybox.frag"
    );

    // Thirdparty config
    stbi_set_flip_vertically_on_load(true);
    Texture boxTexture({"../textures/crate.bmp"}, GL_TEXTURE_2D, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
    Texture floorTexture({"../textures/floor_tiles.bmp"}, GL_TEXTURE_2D, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
    Texture pavementTexture({"../textures/pavement.bmp"}, GL_TEXTURE_2D, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
    Texture skyBoxText = Texture(
        {
            "../textures/skybox/cliff_right.bmp", "../textures/skybox/cliff_left.bmp", "../textures/skybox/cliff_up.bmp",
            "../textures/skybox/cliff_down.bmp", "../textures/skybox/cliff_back.bmp", "../textures/skybox/cliff_front.bmp"
        }, GL_TEXTURE_CUBE_MAP, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE
    );

    Geometry boxGeo = create_cylinder(50, 1.0f, 5.0f);
    Mesh skyboxMesh(create_inverted_box(1000.0f, 1000.0f), skyboxShader);
    skyboxMesh.shaderMaterial.texture = &skyBoxText;

    SceneObject root, cam;
    cam.transform.set_parent(&root.transform, false);
    cam.transform.set_world_position(glm::vec3(0.0f, 0.0f, -5.0f));
    cam.transform.set_world_euler_rotation(glm::vec3(0.0f, 0.0f, 0.0f));
    root.transform.update_transform();

    SceneObject bulletSpawn;
    bulletSpawn.transform.set_parent(&root.transform, false);
    bulletSpawn.transform.set_world_position(glm::vec3(1.0f, 0.0f, 0.0f));
    bulletSpawn.transform.set_world_euler_rotation(glm::vec3(0.0f, 1 * 90.0f, 0.0f));
    root.transform.update_transform();
    
    Bullet bullet(0.0f, -9.8f, false);
    Mesh bulletMesh(boxGeo, basicShader);
    bulletMesh.shaderMaterial.texture = &boxTexture;

    Collider bulletCol = create_sphere_collider(bullet.transform, 0.5f);
    bullet.mesh = &bulletMesh;
    bullet.collider = &bulletCol;

    bullet.transform.set_parent(&root.transform, false);
    bullet.transform.set_world_position(glm::vec3(1.0f, 0.0f, 0.0f));
    bullet.transform.set_world_euler_rotation(glm::vec3(0.0f, 0 * 90.0f, 0.0f));
    //bullet.spawn(bulletSpawn.transform);
    root.transform.update_transform();

    SceneObject floor;
    Mesh floorMesh(boxGeo, basicShader);
    floorMesh.shaderMaterial.texture = &floorTexture;
    floor.mesh = &floorMesh;
    
    Collider floorCol = create_OBB_collider(floor.transform);
    floor.collider = &floorCol;

    floor.transform.set_parent(&root.transform, false);
    floor.transform.set_world_position(glm::vec3(0.0f, -1.0f, 0.0f));
    floor.transform.set_world_scale(glm::vec3(100.0f, 0.5f, 100.0f));
    floor.transform.set_world_euler_rotation(glm::vec3(0.0f, 0.0f, 0.0f));
    root.transform.update_transform();

    physicsEngine.register_entity(*bullet.collider, bullet.transform);
    physicsEngine.register_entity(*floor.collider, floor.transform);
    
    projection = glm::perspective(glm::radians(75.0f), 800.0f/ 600.0f, 0.1f, 1000.0f);
    float old_time = 0.0f;
    float dt = 0.0f;
    // Main loop!
    while (!glfwWindowShouldClose(window))
    {
        dt = glfwGetTime() - old_time;
        old_time = glfwGetTime();

        processInput(window);

        glClearColor(0.106f, 0.118f, 0.169f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (input.x != 0 || input.y != 0 || input.z != 0 || rotInput.x != 0 || rotInput.y != 0)
        {
            auto f = cam.transform.get_front_vector(); // not getting the correct one :(
            auto u = cam.transform.get_up_vector();
            auto r = cam.transform.get_right_vector();

            glm::vec3 dir = r * input.x + f * input.z + u * input.y;
            dir = (input.x != 0 || input.y != 0 || input.z != 0) ? glm::normalize(dir) : glm::vec3(0.0);
            
            glm::quat yawRot = glm::angleAxis(rotInput.y * glm::radians(60.0f) * dt, glm::vec3(0.0f, 1.0f, 0.0f));
            glm::quat pitchRot = glm::angleAxis(rotInput.x * glm::radians(60.0f) * dt, r);
            glm::quat wRot = cam.transform.get_world_rotation() * yawRot;

            glm::vec3 wpos = cam.transform.get_world_position();
            glm::vec3 nextPosition = wpos + dir * 5.0f * dt;
            
            cam.transform.set_world_position(nextPosition);
            cam.transform.set_world_rotation(wRot);
        }

        do_physics();
        bullet.update(dt);
        //logic(root);
        bullet.transform.set_world_euler_rotation(glm::vec3(0.0f, glfwGetTime() * 10.0f, 0.0f));
        
        root.transform.update_transform();
        view = glm::lookAt(
            cam.transform.get_world_position(), 
            cam.transform.get_world_position() + cam.transform.get_front_vector(), 
            cam.transform.get_up_vector()
        );
        
        draw_skybox(skyboxMesh, skyBoxText);
        render_tree(root);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


void windowResizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    projection = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 5000.0f);
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    float xInput = 0.0;
    float yInput = 0.0;
    float zInput = 0.0;
    float yaw = 0.0;
    float pitch = 0.0;
    
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        yInput = 1.0f;
    else if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        yInput = -1.0f;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        xInput = -1.0f;
    else if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        xInput = 1.0f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        zInput = 1.0f;
    else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        zInput = -1.0f;

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        yaw = -1.0f;
    else if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        yaw = 1.0f;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        pitch = -1.0f;
    else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        pitch = 1.0f;

    input.x = xInput;
    input.y = yInput;
    input.z = zInput;
    rotInput.y = yaw;
    rotInput.x = pitch;
}

void do_physics()
{
    physicsEngine.simulate();
}

void render_tree(const SceneObject& root)
{
    for (auto &&c : root.transform.get_children())
    {
        const SceneObject& co = c->get_scene_object();
        if (!co.enabled)
            continue;;

        if (co.mesh != nullptr)
        {   
            co.mesh->shader.use();
            co.mesh->shader.set_mat4f("projection", projection);
            co.mesh->shader.set_mat4f("view", view);
            co.mesh->shader.set_mat4f("model", c->get_model_matrix());
            co.mesh->draw();
        }
        render_tree(co);
    }
}

void draw_skybox(const Mesh& mesh, const Texture& text)
{
    glDepthMask(GL_FALSE);
    mesh.shader.use();
    mesh.shader.set_mat4f("projection", projection);
    mesh.shader.set_mat4f("view", glm::mat4(glm::mat3(view)));
    mesh.draw();
    glDepthMask(GL_TRUE);
}