#include "thirdparty/glad/include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include "thirdparty/glm/gtc/matrix_transform.hpp"
#include "thirdparty/glm/gtc/type_ptr.hpp"
#include "thirdparty/stb/stb_image.h"
#include "thirdparty/glm/glm.hpp"

#include "src/scene_graph/sceneobject.hpp"
#include "src/scene_graph/transform.hpp"
#include "src/scene_graph/component.hpp"

#include "src/rendering/rendering_engine.hpp"
#include "src/rendering/lights.hpp"

#include "src/gl_utils/shader.h"

#include "src/textures/texture.hpp"
#include "src/mesh/geometry.hpp"
#include "src/mesh/mesh.hpp"

#include "src/physics/physic_engine.hpp"
#include "src/physics/collider_component.hpp"

#include "src/game/components/destroyable_component.hpp"

#include "src/game/bullet.hpp"
#include "src/game/tank.hpp"
#include "src/input/input.hpp"

#include "src/resource_management/resource_manager.hpp"
#include "src/resource_management/texture_loader.hpp"

// Window resize handler
void windowResizeCallback(GLFWwindow* window, int width, int height);
// Input processing
void processInput(GLFWwindow* window);

glm::vec3 input(0.0f);
glm::vec2 rotInput(0.0f);
glm::mat4 projection, view;

void logic(const SceneObject& root);
void render_tree(const SceneObject& root);
void draw_skybox(const Mesh& mesh);

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


    TextureLoader txLoader;
    ResourceManager<Texture> txManager(txLoader);
    Geometry boxGeo = create_box(1.0f, 1.0f, 1.0f);

    // Creating meshes
    Mesh skyboxMesh(create_inverted_box(1000.0f, 1000.0f), skyboxShader);
    skyboxMesh.shaderMaterial.albedo = txManager.load_resource("../textures/skybox/cliff.bmp");

    Mesh bulletMesh(boxGeo, basicShader);
    bulletMesh.shaderMaterial.albedo = txManager.load_resource("../textures/crate.bmp");

    Mesh floorMesh(boxGeo, basicShader);
    floorMesh.shaderMaterial.albedo = txManager.load_resource("../textures/floor.bmp");

    Mesh boxMesh(boxGeo, basicShader);
    boxMesh.shaderMaterial.albedo = txManager.load_resource("../textures/crate.bmp");

    Mesh sphereMesh(create_sphere(12, 12, 0.5f), basicShader);
    sphereMesh.shaderMaterial.tint = glm::vec3(1.0f, 0.0f, 1.0f);

    // Creating SceneObjects
    SceneObject root, cam;
    root.transform.update_transform();

#pragma region Tank setup
    // Tank set up
    Tank tank(root, basicShader);
    tank.mesh->shaderMaterial.albedo = txManager.load_resource("../textures/tank.bmp");
    tank.transform.set_world_position(glm::vec3(0.0f, 0.0f, 0.0f));
    tank.transform.set_world_euler_rotation(glm::vec3(0.0f, 0.0f, 0.0f));
    root.transform.update_transform();

    cam.transform.set_parent(&tank.transform, false);
    cam.transform.set_world_position(glm::vec3(0.0f, 2.0f, -3.0f));
    cam.transform.set_world_euler_rotation(glm::vec3(0.0f, 0.0f, 0.0f));
    tank.transform.update_transform();
#pragma endregion


#pragma region Environment
    // Setting main light
    Light mainLight;
    mainLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
    mainLight.intensity = 1.0f;
    mainLight.transform.set_parent(&root.transform, false);
    mainLight.transform.set_world_position(glm::vec3(1.0f, 10.0f, -1.0f));
    root.transform.update_transform();

    SceneObject floor;
    floor.mesh = &floorMesh;
    floor.mesh->shaderMaterial.texUVscales = glm::vec3(100.0f);
    floor.transform.set_parent(&root.transform, false);
    floor.transform.set_world_position(glm::vec3(0.0f, -1.0f, 0.0f));
    floor.transform.set_world_scale(glm::vec3(1000.0f, 0.5f, 1000.0f));
    floor.transform.set_world_euler_rotation(glm::vec3(0.0f, 0.0f, 0.0f));
    root.transform.update_transform();

    SceneObject sphere1;
    sphere1.mesh = &sphereMesh;
    sphere1.transform.set_parent(&root.transform, false);
    sphere1.transform.set_world_position(glm::vec3(-8.0f, 1.0f, 8.0f));
    sphere1.transform.set_world_euler_rotation(glm::vec3(0.0f, 0.0f, 0.0f));
    sphere1.add_component(*new DestroyableComponent(&sphere1));

    SceneObject sphere2;
    sphere2.mesh = &sphereMesh;
    sphere2.transform.set_parent(&root.transform, false);
    sphere2.transform.set_world_position(glm::vec3(-8.0f, 1.0f, 6.0f));
    sphere2.transform.set_world_euler_rotation(glm::vec3(0.0f, 0.0f, 0.0f));
    sphere2.add_component(*new DestroyableComponent(&sphere2));

    SceneObject sphere3;
    sphere3.mesh = &sphereMesh;
    sphere3.transform.set_parent(&root.transform, false);
    sphere3.transform.set_world_position(glm::vec3(-8.0f, 1.0f, 4.0f));
    sphere3.transform.set_world_euler_rotation(glm::vec3(0.0f, 0.0f, 0.0f));
    sphere3.add_component(*new DestroyableComponent(&sphere3));

    SceneObject sphere4;
    sphere4.mesh = &sphereMesh;
    sphere4.transform.set_parent(&root.transform, false);
    sphere4.transform.set_world_position(glm::vec3(-8.0f, 1.0f, 2.0f));
    sphere4.transform.set_world_euler_rotation(glm::vec3(0.0f, 0.0f, 0.0f));
    sphere4.add_component(*new DestroyableComponent(&sphere4));

    SceneObject box1;
    box1.mesh = &boxMesh;
    box1.transform.set_parent(&root.transform, false);
    box1.transform.set_world_position(glm::vec3(10.0f, 0.2f, 10.0f));
    box1.transform.set_world_euler_rotation(glm::vec3(0.0f, 0.0f, 0.0f));
    box1.transform.set_world_scale(glm::vec3(2.0f, 2.0f, 2.0f));
    box1.add_component(*new DestroyableComponent(&box1));

    SceneObject box2;
    box2.mesh = &boxMesh;
    box2.transform.set_parent(&root.transform, false);
    box2.transform.set_world_position(glm::vec3(7.0f, 1.2f, 10.0f));
    box2.transform.set_world_euler_rotation(glm::vec3(45.0f, 0.0f, 0.0f));
    box2.transform.set_world_scale(glm::vec3(2.0f, 2.0f, 2.0f));
    box2.add_component(*new DestroyableComponent(&box2));

    SceneObject box3;
    box3.mesh = &boxMesh;
    box3.transform.set_parent(&root.transform, false);
    box3.transform.set_world_position(glm::vec3(4.0f, 2.2f, 10.0f));
    box3.transform.set_world_euler_rotation(glm::vec3(0.0f, 45.0f, 0.0f));
    box3.transform.set_world_scale(glm::vec3(2.0f, 2.0f, 2.0f));
    box3.add_component(*new DestroyableComponent(&box3));

    SceneObject box4;
    box4.mesh = &boxMesh;
    box4.transform.set_parent(&root.transform, false);
    box4.transform.set_world_position(glm::vec3(1.0f, 3.2f, 10.0f));
    box4.transform.set_world_euler_rotation(glm::vec3(0.0f, 0.0f, 45.0f));
    box4.transform.set_world_scale(glm::vec3(2.0f, 2.0f, 2.0f));
    box4.add_component(*new DestroyableComponent(&box4));

    SceneObject box5;
    box5.mesh = &boxMesh;
    box5.transform.set_parent(&root.transform, false);
    box5.transform.set_world_position(glm::vec3(-2.0f, 4.2f, 10.0f));
    box5.transform.set_world_euler_rotation(glm::vec3(45.0f, 45.0f, 0.0f));
    box5.transform.set_world_scale(glm::vec3(2.0f, 2.0f, 2.0f));
    box5.add_component(*new DestroyableComponent(&box5));

    SceneObject box6;
    box6.mesh = &boxMesh;
    box6.transform.set_parent(&root.transform, false);
    box6.transform.set_world_position(glm::vec3(-5.0f, 4.2f, 10.0f));
    box6.transform.set_world_euler_rotation(glm::vec3(0.0f, 45.0f, 45.0f));
    box6.transform.set_world_scale(glm::vec3(2.0f, 2.0f, 2.0f));
    box6.add_component(*new DestroyableComponent(&box6));


#pragma endregion


#pragma region Physic setup
    BoxCollider floorCollider(&floor, glm::vec3(1.0f));
    floor.add_component(floorCollider);

    BoxCollider box1Collider(&box1, glm::vec3(1.0f));
    box1.add_component(box1Collider);

    BoxCollider box2Collider(&box2, glm::vec3(1.0f));
    box2.add_component(box2Collider);

    BoxCollider box3Collider(&box3, glm::vec3(1.0f));
    box3.add_component(box3Collider);

    BoxCollider box4Collider(&box4, glm::vec3(1.0f));
    box4.add_component(box4Collider);

    BoxCollider box5Collider(&box5, glm::vec3(1.0f));
    box5.add_component(box5Collider);

    BoxCollider box6Collider(&box6, glm::vec3(1.0f));
    box6.add_component(box6Collider);

    SphereCollider sphere1Collider(&sphere1, 0.5f);
    sphere1.add_component(sphere1Collider);

    SphereCollider sphere2Collider(&sphere2, 0.5f);
    sphere2.add_component(sphere2Collider);

    SphereCollider sphere3Collider(&sphere3, 0.5f);
    sphere3.add_component(sphere3Collider);

    SphereCollider sphere4Collider(&sphere4, 0.5f);
    sphere4.add_component(sphere4Collider);

#pragma endregion

#pragma region Rendering
    // Setting main camera and registering lights
    renderEngine.set_main_camera(&cam.transform);
    renderEngine.register_light(mainLight);
#pragma endregion

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
        draw_skybox(skyboxMesh);
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

void draw_skybox(const Mesh& mesh)
{
    glDepthMask(GL_FALSE);
    mesh.shader->use();
    mesh.draw();
    glDepthMask(GL_TRUE);
}