#include "thirdparty/glad/include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include "thirdparty/glm/gtc/matrix_transform.hpp"
#include "thirdparty/glm/gtc/type_ptr.hpp"
#include "thirdparty/stb/stb_image.h"
#include "thirdparty/glm/glm.hpp"

#include "src/rendering/rendering_engine.hpp"
#include "src/rendering/lights.hpp"
#include "src/physics/physic_engine.hpp"
#include "src/scene_graph/transform.hpp"
#include "src/textures/texture.hpp"
#include "src/mesh/geometry.hpp"
#include "src/mesh/mesh.hpp"
#include "src/gl_utils/shader.h"
#include "src/game/bullet.hpp"
#include "src/game/tank.hpp"
#include "src/game/obstacle.hpp"
#include "src/input/input.hpp"


// Window resize handler
void windowResizeCallback(GLFWwindow* window, int width, int height);
// Input processing
void processInput(GLFWwindow* window);


glm::vec3 input(0.0f);
glm::vec2 rotInput(0.0f);
glm::mat4 projection, view;

void logic(const SceneObject& root);
void render_tree(const SceneObject& root);
void draw_skybox(const Mesh& mesh, const Texture& text);

int main()
{
    // Window initializantion
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
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

    InputManager *input_mgr;
    input_mgr = input_mgr->get_instance();
    input_mgr->set_window(window);
    PhysicEngine physicsEngine;
    RenderingEngine renderEngine(800.0f, 600.0f, 75.0f);

    // Open GL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    // Thirdparty config
    stbi_set_flip_vertically_on_load(true);

    // Shaders and texture loading
    gl_utils::shader_program basicShader = gl_utils::shader_program(
        "../shader_files/basic.vert",
        "../shader_files/basic.frag"
    );
    gl_utils::shader_program skyboxShader = gl_utils::shader_program(
        "../shader_files/skybox.vert",
        "../shader_files/skybox.frag"
    );

    Texture boxTexture({"../textures/crate.bmp"}, GL_TEXTURE_2D, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
    Texture floorTexture({"../textures/floor.bmp"}, GL_TEXTURE_2D, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
    Texture pavementTexture({"../textures/pavement.bmp"}, GL_TEXTURE_2D, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
    Texture skyBoxText = Texture(
        {
            "../textures/skybox/cliff_front.bmp", "../textures/skybox/cliff_back.bmp", "../textures/skybox/cliff_down.bmp",
            "../textures/skybox/cliff_up.bmp", "../textures/skybox/cliff_right.bmp", "../textures/skybox/cliff_left.bmp"
        }, GL_TEXTURE_CUBE_MAP, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE
    );

    Texture tankTexture({"../textures/tank.bmp"}, GL_TEXTURE_2D, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);

    Geometry boxGeo = create_box(1.0f, 1.0f, 1.0f);

    // Creating meshes
    Mesh skyboxMesh(create_inverted_box(1000.0f, 1000.0f), skyboxShader);
    skyboxMesh.shaderMaterial.texture = &skyBoxText;

    Mesh bulletMesh(boxGeo, basicShader);
    bulletMesh.shaderMaterial.texture = &boxTexture;

    Mesh floorMesh(boxGeo, basicShader);
    floorMesh.shaderMaterial.texture = &floorTexture;

    // Creating SceneObjects
    SceneObject root, cam;
    root.transform.update_transform();

    SceneObject bulletSpawn;
    bulletSpawn.transform.set_parent(&root.transform, false);
    bulletSpawn.transform.set_world_position(glm::vec3(1.0f, 0.0f, 0.0f));
    bulletSpawn.transform.set_world_euler_rotation(glm::vec3(0.0f, 90.0f, 0.0f));
    root.transform.update_transform();

    Obstacle sphere = create_sphere_obstacle(root.transform, 30, 20, 2.0f, basicShader, boxTexture);
    sphere.transform.set_world_position(glm::vec3(-5.0f, 2.0f, 3.0f));
    sphere.transform.set_world_euler_rotation(glm::vec3(0.0f, 0.0f, 0.0f));
    sphere.enabled = true;
    root.transform.update_transform();

    Obstacle box = create_box_obstacle(root.transform, 1.0f, 1.0f, 1.0f, basicShader, boxTexture);
    box.transform.set_world_position(glm::vec3(7.0f, 2.0f, -6.0f));
    box.transform.set_world_euler_rotation(glm::vec3(0.0f, 0.0f, 0.0f));
    box.enabled = true;
    root.transform.update_transform();

    Tank tank(root, basicShader);
    tank.mesh->shaderMaterial.texture = &tankTexture;
    tank.transform.set_world_position(glm::vec3(0.0f, 0.0f, 0.0f));
    tank.transform.set_world_euler_rotation(glm::vec3(0.0f, 0.0f, 0.0f));
    root.transform.update_transform();

    cam.transform.set_parent(&tank.transform, false);
    cam.transform.set_world_position(glm::vec3(0.0f, 2.0f, -3.0f));
    cam.transform.set_world_euler_rotation(glm::vec3(0.0f, 0.0f, 0.0f));
    tank.transform.update_transform();

    SceneObject floor;
    floor.mesh = &floorMesh;

    floor.transform.set_parent(&root.transform, false);
    floor.transform.set_world_position(glm::vec3(0.0f, -1.0f, 0.0f));
    floor.transform.set_world_scale(glm::vec3(100.0f, 0.5f, 100.0f));
    floor.transform.set_world_euler_rotation(glm::vec3(0.0f, 0.0f, 0.0f));
    root.transform.update_transform();
    
    // Setting colliders
    Collider floorCol = create_OBB_collider(floor.transform);
    // Collider bulletCol = create_sphere_collider(bullet.transform, 0.5f);
    floor.collider = &floorCol;
    // bullet.collider = &bulletCol;

    // Setting main light
    Light mainLight;
    mainLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
    mainLight.intensity = 2.0f;

    mainLight.transform.set_parent(&root.transform, false);
    mainLight.transform.set_world_position(glm::vec3(1.0f, 10.0f, -1.0f));
    root.transform.update_transform();

    // Registering colliders
    physicsEngine.register_entity(*floor.collider, floor.transform);
    physicsEngine.register_entity(*sphere.collider, sphere.transform);
    physicsEngine.register_entity(*box.collider, box.transform);
    for (int i = 0; i < 3; i++) {
        physicsEngine.register_entity(*tank.bullets[i]->collider, tank.bullets[i]->transform);
    }

    // Setting main camera and registering lights
    renderEngine.set_main_camera(&cam.transform);
    renderEngine.register_light(mainLight);

    float old_time = 0.0f;
    float dt = 0.0f;
    // Main loop!

    while (!glfwWindowShouldClose(window))
    {
        dt = glfwGetTime() - old_time;

        old_time = glfwGetTime();

        glClearColor(0.106f, 0.118f, 0.169f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        tank.update(dt);
        tank.update_bullets(dt);

        root.transform.update_transform();
        physicsEngine.simulate();
        draw_skybox(skyboxMesh, skyBoxText);
        renderEngine.render_tree(root, true);

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (input_mgr->key_is_pressed(GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, true);
            break;
        }
    }

    glfwTerminate();
    return 0;
}


void windowResizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    projection = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 5000.0f);
}

void draw_skybox(const Mesh& mesh, const Texture& text)
{
    glDepthMask(GL_FALSE);
    mesh.shader->use();
    mesh.draw();
    glDepthMask(GL_TRUE);
}