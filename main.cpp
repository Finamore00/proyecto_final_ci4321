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

int main()
{
    // Window initializantion
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    gl_utils::shader_program basicShader = gl_utils::shader_program(
        "../shader_files/basic.v",
        "../shader_files/basic.f");

    Texture boxTexture("../textures/container.jpg", GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
    Geometry boxGeo = createBox(1.0f, 1.0f);

    // Open GL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    // Thirdparty config
    stbi_set_flip_vertically_on_load(true);

    SceneObject root, cam;
    root.transform.updateTransform();

    cam.transform.setParent(&root.transform, false);
    cam.transform.setWorldPosition(glm::vec3(0.0f, 0.0f, 10.0f));
    cam.transform.setWorldEulerRotation(glm::vec3(0.0f, 180.0f, 0.0f));
    root.transform.updateTransform();

    SceneObject bulletSpawn;
    bulletSpawn.transform.setParent(&root.transform, false);
    bulletSpawn.transform.setWorldEulerRotation(glm::vec3(0.0f, 90.0f, 0.0f));
    root.transform.updateTransform();
    
    Bullet bullet(2.0f, -9.8f, false);
    Mesh boxMesh(boxGeo, basicShader);
    Collider bulletCol;
    bullet.mesh = &boxMesh;
    bullet.collider = &bulletCol;
    bullet.collider->type = SPHERE_COLLIDER;
    bullet.collider->shape = {.sphere= {0.5f, bulletSpawn.transform.getWorldPosition()}}; 

    bullet.transform.setParent(&root.transform, false);
    root.transform.updateTransform();
    bullet.spawn(bulletSpawn.transform);

    SceneObject wall;
    Collider wallCol;
    wall.transform.setParent(&root.transform, false);
    wall.transform.setWorldPosition(glm::vec3(5.0f, 0.0f, 0.0f));
    root.transform.updateTransform();
    wall.mesh = &boxMesh;
    wall.collider = &wallCol;
    wall.collider->type = SPHERE_COLLIDER;
    wall.collider->shape = {.sphere= {0.5f, wall.transform.getWorldPosition()}};

    physicsEngine.register_entity(*bullet.collider, bullet.transform);
    physicsEngine.register_entity(*wall.collider, wall.transform);
    
    projection = glm::perspective(glm::radians(45.0f), 800.0f/ 600.0f, 0.1f, 1000.0f);

    float old_time, dt;
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
            auto f = cam.transform.getFrontVector(); // not getting the correct one :(
            auto u = cam.transform.getUpVector();
            auto r = cam.transform.getRightVector();

            glm::vec3 dir = r * input.x + f * input.z + u * input.y;
            dir = (input.x != 0 || input.y != 0 || input.z != 0) ? glm::normalize(dir) : glm::vec3(0.0);
            
            glm::quat yawRot = glm::angleAxis(rotInput.y * glm::radians(60.0f) * dt, glm::vec3(0.0f, 1.0f, 0.0f));
            glm::quat pitchRot = glm::angleAxis(rotInput.x * glm::radians(60.0f) * dt, r);
            glm::quat wRot = cam.transform.getWorldRotation() * yawRot;

            std::cout << dir.x << "," << dir.y << "," << dir.z << std::endl;

            glm::vec3 wpos = cam.transform.getWorldPosition();
            glm::vec3 nextPosition = wpos + dir * 5.0f * dt;
            
            cam.transform.setWorldPosition(nextPosition);
            cam.transform.setWorldRotation(wRot);
        }

        do_physics();
        //bullet.update(dt);
        //logic(root);
        
        root.transform.updateTransform();
        view = glm::lookAt(
            cam.transform.getWorldPosition(), 
            cam.transform.getWorldPosition() + cam.transform.getFrontVector(), 
            cam.transform.getUpVector()
        );
        
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
    for (auto &&c : root.transform.getChildren())
    {
        const SceneObject& co = c->getSceneObject();
        if (!co.enabled)
            continue;;

        if (co.mesh != nullptr)
        {   
            auto wpos = co.transform.getWorldPosition();
            co.mesh->shader.set_mat4f("projection", projection);
            co.mesh->shader.set_mat4f("view", view);
            co.mesh->shader.set_mat4f("model", c->getModelMatrix());
            c->getSceneObject().mesh->draw();
        }
        render_tree(co);
    }
}