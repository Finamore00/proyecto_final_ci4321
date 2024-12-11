#include "thirdparty/glad/include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <functional>
#include <iostream>

#include <chrono>
#include <random>

#include "thirdparty/glm/gtc/matrix_transform.hpp"
#include "thirdparty/glm/gtc/type_ptr.hpp"
#include "thirdparty/stb/stb_image.h"
#include "thirdparty/glm/glm.hpp"

#include "src/scene_graph/sceneobject.hpp"
#include "src/scene_graph/transform.hpp"
#include "src/scene_graph/component.hpp"

#include "src/rendering/rendering_engine.hpp"
#include "src/rendering/lights.hpp"

#include "src/gl_utils/shader.hpp"

#include "src/textures/texture_loader.hpp"
#include "src/textures/texture.hpp"

#include "src/mesh/model_loader.hpp"
#include "src/mesh/geometry.hpp"
#include "src/mesh/mesh.hpp"

#include "src/physics/physic_engine.hpp"
#include "src/physics/collider_component.hpp"

#include "src/game/components/obstacle_counter_component.hpp"
#include "src/game/components/destroyable_component.hpp"
#include "src/game/speedo_meter_component.hpp"
#include "src/game/firetruck_component.hpp"
#include "src/game/bullet_component.hpp"
#include "src/game/firetruck_cannon.hpp"

#include "src/input/input.hpp"

#include "src/resource_management/resource_manager.hpp"

#include "src/ui/sprites/sprite_component.hpp"
#include "src/ui/font/font_component.hpp"
#include "src/ui/font/font_loader.hpp"

#include "src/particles/particle_emitter.hpp"

#include "src/scene_graph/logic_engine.hpp"

// Window resize handler
void windowResizeCallback(GLFWwindow *window, int width, int height);
// Input processing
void processInput(GLFWwindow *window);

glm::vec3 input(0.0f);
glm::vec2 rotInput(0.0f);

RenderingEngine *re_ptr;

int main()
{
    srand(std::chrono::system_clock::now().time_since_epoch().count());
    // Window initializantion
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL Triangle", NULL, NULL);
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
    RenderingEngine renderEngine(*window, 800.0f, 600.0f, 75.0f);
    LogicEngine logicEngine;
    re_ptr = &renderEngine;

    // Open GL settings
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    // Thirdparty config
    stbi_set_flip_vertically_on_load(true);

    // Shaders and texture loading
    gl_utils::shader_program basicShader = gl_utils::shader_program(
        "../shader_files/basic.vert",
        "../shader_files/basic.frag");

    gl_utils::shader_program skyboxShader = gl_utils::shader_program(
        "../shader_files/skybox.vert",
        "../shader_files/skybox.frag");

    gl_utils::shader_program fontShader = gl_utils::shader_program(
        "../shader_files/ui/ui.vert",
        "../shader_files/ui/font.frag");

    gl_utils::shader_program spriteShader = gl_utils::shader_program(
        "../shader_files/ui/ui.vert",
        "../shader_files/ui/ui.basic.frag");
        
    gl_utils::shader_program normalMapShader = gl_utils::shader_program(
        "../shader_files/normalmap.vert",
        "../shader_files/normalmap.frag"
    );

    gl_utils::shader_program particleShader = gl_utils::shader_program(
        "../shader_files/particles.vert", 
        "../shader_files/particles.frag");

    TextureLoader txLoader;
    ResourceManager<Texture> txManager(txLoader);

    FontLoader fontLoader(txManager);
    ResourceManager<FontAtlas> fontManager(fontLoader);

    Geometry boxGeo = create_box(1.0f, 1.0f, 1.0f);

#pragma region Model loading
    // Creating meshes
    Mesh skyboxMesh(create_inverted_box(1000.0f, 1000.0f), skyboxShader);
    skyboxMesh.shaderMaterial.albedo = txManager.load_resource("../textures/skybox/cliff.bmp");
    std::shared_ptr<Texture> skyboxTx = txManager.load_resource("../textures/skybox/cliff.bmp");

    Mesh bulletMesh(create_sphere(12, 12, 0.5f), basicShader);
    bulletMesh.shaderMaterial.tint = glm::vec3(0.0f, 0.0f, 1.0f);

    Mesh floorMesh(boxGeo, normalMapShader);
    floorMesh.shaderMaterial.albedo = txManager.load_resource("../textures/stonefloor.png");
    floorMesh.shaderMaterial.normal_map = txManager.load_resource("../textures/stonefloor_normal.jpg");

    Mesh boxMesh(boxGeo, normalMapShader);
    boxMesh.shaderMaterial.albedo = txManager.load_resource("../textures/brickwall.jpg");
    boxMesh.shaderMaterial.normal_map = txManager.load_resource("../textures/brickwall_normal.jpg");

    Mesh sphereMesh(create_sphere(12, 12, 1.2f), normalMapShader);
    sphereMesh.shaderMaterial.albedo = txManager.load_resource("../textures/stonefloor.png");
    sphereMesh.shaderMaterial.normal_map = txManager.load_resource("../textures/stonefloor_normal.jpg");

    Mesh lightMesh(create_sphere(12, 12, 0.5f), basicShader);
    lightMesh.shaderMaterial.tint = glm::vec3(1.0f);
    lightMesh.shaderMaterial.ambient = glm::vec3(1.0f);

    ModelLoader mLoader;
    std::shared_ptr<Model> firetruckModel = mLoader.load_resource("../models/firetruck.gltf");
    std::shared_ptr<Model> turretModel = mLoader.load_resource("../models/firetruck_turret_mount.gltf");
    std::shared_ptr<Model> cannonModel = mLoader.load_resource("../models/firetruck_cannon.gltf");
    for (int i = 0; i < firetruckModel.get()->mesh_number(); ++i)
    {
        firetruckModel.get()->get_mesh(i)->set_shader(basicShader);
        firetruckModel.get()->get_mesh(i)->shaderMaterial.albedo = txLoader.load_resource("../textures/firetruck.png");
    }

    turretModel.get()->get_mesh(0)->set_shader(basicShader);
    turretModel.get()->get_mesh(0)->shaderMaterial.albedo = txLoader.load_resource("../textures/firetruck.png");

    cannonModel.get()->get_mesh(0)->set_shader(basicShader);
    cannonModel.get()->get_mesh(0)->shaderMaterial.albedo = txLoader.load_resource("../textures/firetruck.png");

#pragma endregion

    // Creating SceneObjects
    SceneObject root, cam;
    root.transform.update_transform();

#pragma region Bullets setup
    SceneObject b1, b2, b3;
    b1.mesh = &bulletMesh;
    b2.mesh = &bulletMesh;
    b3.mesh = &bulletMesh;

    b1.add_component<BulletComponent>(&b1, 10.0f, -9.8f, false);
    b2.add_component<BulletComponent>(&b2, 10.0f, -9.8f, false);
    b3.add_component<BulletComponent>(&b3, 10.0f, -9.8f, false);

    b1.add_component<SphereCollider>(&b1, 0.5f);
    b2.add_component<SphereCollider>(&b2, 0.5f);
    b3.add_component<SphereCollider>(&b3, 0.5f);

    b1.transform.set_parent(&root.transform, false);
    b2.transform.set_parent(&root.transform, false);
    b3.transform.set_parent(&root.transform, false);

    b1.transform.set_world_scale(glm::vec3(0.5f, 0.5f, 0.5f));
    b2.transform.set_world_scale(glm::vec3(0.5f, 0.5f, 0.5f));
    b3.transform.set_world_scale(glm::vec3(0.5f, 0.5f, 0.5f));

    b1.active = false;
    b2.active = false;
    b3.active = false;

#pragma endregion

#pragma region Tank setup
    SceneObject firetruck, firetruckVisual, turret, cannonPivot, cannon, bulletSpawn, cannonVfx;
    firetruck.transform.set_parent(&root.transform, false);
    firetruck.transform.set_world_position(glm::vec3(0.0f, 0.0f, 0.0f));
    firetruck.transform.set_world_euler_rotation(glm::vec3(0.0f, 0.0f, 0.0f));
    firetruck.add_component<FiretruckComponent>(&firetruck);
    root.transform.update_transform();

    firetruckVisual.transform.set_parent(&firetruck.transform, false);
    firetruckVisual.transform.set_world_euler_rotation(glm::vec3(0.0f, -90.0f, 0.0f));
    firetruckVisual.transform.set_world_scale(glm::vec3(0.6f));
    firetruckVisual.model = firetruckModel;

    cannonVfx.transform.set_parent(&bulletSpawn.transform, false);
    cannonVfx.transform.update_transform();
    cannonVfx.add_component<ParticleEmitter>(&cannonVfx, particleShader);

    std::weak_ptr<ParticleEmitter> cannonEmitter = cannonVfx.get_component<ParticleEmitter>();
    if (auto e = cannonEmitter.lock())
    {
        e->set_shape<ConeEmissor>(0.5f, 1.0f);
        e->set_timer<BurstEmissionTime>(100u, 0.0f, false);
        e->set_texture(txManager.load_resource("../textures/particle.png"));
        e->set_life_time(0.7f);
        e->set_start_color(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
        e->set_end_color(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
        e->set_start_size(0.2f);
        e->set_start_speed(20.0f);
    }

    turret.transform.set_parent(&firetruck.transform, false);
    turret.transform.set_local_position(glm::vec3(0.0f, 1.0f, 0.0f));
    turret.add_component<FiretruckCannonComponent>(&turret, cannonPivot.transform, bulletSpawn.transform, (std::vector<SceneObject*>){&b1, &b2, &b3}, cannonEmitter);
    turret.model = turretModel;

    cannonPivot.transform.set_parent(&turret.transform, false);
    cannonPivot.transform.set_local_position(glm::vec3(0.0f, 0.25f, 0.0f));
    cannon.transform.set_parent(&cannonPivot.transform, false);
    cannon.model = cannonModel;

    bulletSpawn.transform.set_parent(&cannon.transform, false);
    bulletSpawn.transform.set_local_position(glm::vec3(0.0f, 0.0f, 1.0f));

    cam.transform.set_parent(&firetruck.transform, false);
    cam.transform.set_world_position(glm::vec3(0.0f, 2.0f, -7.0f));
    cam.transform.set_world_euler_rotation(glm::vec3(0.0f, 0.0f, 0.0f));
#pragma endregion

#pragma region Environment
    // Setting main light
    Light mainLight;
    mainLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
    mainLight.intensity = 1.5f;
    mainLight.transform.set_parent(&root.transform, false);
    mainLight.transform.set_world_position(glm::vec3(0.0f, 5.0f, 0.0f));
    mainLight.type = LightSourceType::Directional;
    mainLight.set_direction(glm::vec3(0.0, -1.0, 0.0));
    mainLight.intensity = 0.5f;
    root.transform.update_transform();

    Light firetruck_headlight;
    firetruck_headlight.transform.set_parent(&firetruck.transform, true);
    firetruck_headlight.transform.set_local_position(glm::vec3(0.0f, 0.0f, 1.0f));
    firetruck_headlight.type = LightSourceType::Spot;
    firetruck_headlight.set_direction(glm::vec3(0.0f, 0.0f, 1.0f));
    firetruck_headlight.intensity = 0.7;
    firetruck_headlight.disable_light();
    firetruck_headlight.set_cutoff(0.8);
    firetruck.transform.update_transform();

    Light secondary_light;
    secondary_light.transform.set_parent(&root.transform, false);
    secondary_light.transform.set_world_position(glm::vec3(20.0, 3.0, 0.0));
    secondary_light.mesh = &lightMesh;
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
    sphere1.model = firetruckModel;
    sphere1.transform.set_parent(&root.transform, false);
    sphere1.transform.set_world_position(glm::vec3(30.0f, 2.0f, 0.0f));
    sphere1.transform.set_world_euler_rotation(glm::vec3(0.0f, 45.0f, 0.0f));
    sphere1.add_component<DestroyableComponent>(&sphere1);

    SceneObject sphere2;
    sphere2.mesh = &sphereMesh;
    sphere2.transform.set_parent(&root.transform, false);
    sphere2.transform.set_world_position(glm::vec3(-8.0f, 1.0f, 6.0f));
    sphere2.transform.set_world_euler_rotation(glm::vec3(0.0f, 0.0f, 0.0f));
    sphere2.add_component<DestroyableComponent>(&sphere2);

    SceneObject sphere3;
    sphere3.mesh = &sphereMesh;
    sphere3.transform.set_parent(&root.transform, false);
    sphere3.transform.set_world_position(glm::vec3(-8.0f, 1.0f, 3.0f));
    sphere3.transform.set_world_euler_rotation(glm::vec3(0.0f, 0.0f, 0.0f));
    sphere3.add_component<DestroyableComponent>(&sphere3);

    SceneObject sphere4;
    sphere4.mesh = &sphereMesh;
    sphere4.transform.set_parent(&root.transform, false);
    sphere4.transform.set_world_position(glm::vec3(-8.0f, 1.0f, 0.0f));
    sphere4.transform.set_world_euler_rotation(glm::vec3(0.0f, 0.0f, 0.0f));
    sphere4.add_component<DestroyableComponent>(&sphere4);

    SceneObject box1;
    box1.mesh = &boxMesh;
    box1.transform.set_parent(&root.transform, false);
    box1.transform.set_world_position(glm::vec3(10.0f, 0.2f, 10.0f));
    box1.transform.set_world_euler_rotation(glm::vec3(0.0f, 0.0f, 0.0f));
    box1.transform.set_world_scale(glm::vec3(2.0f, 2.0f, 2.0f));
    box1.add_component<DestroyableComponent>(&box1);

    SceneObject box2;
    box2.mesh = &boxMesh;
    box2.transform.set_parent(&root.transform, false);
    box2.transform.set_world_position(glm::vec3(7.0f, 1.2f, 10.0f));
    box2.transform.set_world_euler_rotation(glm::vec3(45.0f, 0.0f, 0.0f));
    box2.transform.set_world_scale(glm::vec3(2.0f, 2.0f, 2.0f));
    box2.add_component<DestroyableComponent>(&box2);

    SceneObject box3;
    box3.mesh = &boxMesh;
    box3.transform.set_parent(&root.transform, false);
    box3.transform.set_world_position(glm::vec3(4.0f, 2.2f, 10.0f));
    box3.transform.set_world_euler_rotation(glm::vec3(0.0f, 45.0f, 0.0f));
    box3.transform.set_world_scale(glm::vec3(2.0f, 2.0f, 2.0f));
    box3.add_component<DestroyableComponent>(&box3);

    SceneObject box4;
    box4.mesh = &boxMesh;
    box4.transform.set_parent(&root.transform, false);
    box4.transform.set_world_position(glm::vec3(1.0f, 3.2f, 10.0f));
    box4.transform.set_world_euler_rotation(glm::vec3(0.0f, 0.0f, 45.0f));
    box4.transform.set_world_scale(glm::vec3(2.0f, 2.0f, 2.0f));
    box4.add_component<DestroyableComponent>(&box4);

    SceneObject box5;
    box5.mesh = &boxMesh;
    box5.transform.set_parent(&root.transform, false);
    box5.transform.set_world_position(glm::vec3(-2.0f, 4.2f, 10.0f));
    box5.transform.set_world_euler_rotation(glm::vec3(45.0f, 45.0f, 0.0f));
    box5.transform.set_world_scale(glm::vec3(2.0f, 2.0f, 2.0f));
    box5.add_component<DestroyableComponent>(&box5);

    SceneObject box6;
    box6.mesh = &boxMesh;
    box6.transform.set_parent(&root.transform, false);
    box6.transform.set_world_position(glm::vec3(-5.0f, 4.2f, 10.0f));
    box6.transform.set_world_euler_rotation(glm::vec3(0.0f, 45.0f, 45.0f));
    box6.transform.set_world_scale(glm::vec3(2.0f, 2.0f, 2.0f));
    box6.add_component<DestroyableComponent>(&box6);


#pragma endregion

#pragma region Physic setup
    floor.add_component<BoxCollider>(&floor, glm::vec3(1.0f));

    box1.add_component<BoxCollider>(&box1, glm::vec3(1.0f));
    box2.add_component<BoxCollider>(&box2, glm::vec3(1.0f));
    box3.add_component<BoxCollider>(&box3, glm::vec3(1.0f));
    box4.add_component<BoxCollider>(&box4, glm::vec3(1.0f));
    box5.add_component<BoxCollider>(&box5, glm::vec3(1.0f));
    box6.add_component<BoxCollider>(&box6, glm::vec3(1.0f));

    sphere1.add_component<SphereCollider>(&sphere1, 0.5f);
    sphere2.add_component<SphereCollider>(&sphere2, 0.5f);
    sphere3.add_component<SphereCollider>(&sphere3, 0.5f);
    sphere4.add_component<SphereCollider>(&sphere4, 0.5f);

#pragma endregion

#pragma region UI
    SceneObject ui_root;
    SceneObject obstacle_counter;
    SceneObject speedoMeter, speedGaugePivot, speedGauge;

    obstacle_counter.transform.set_parent(&ui_root.transform, false);
    obstacle_counter.add_component<FontComponent>(&obstacle_counter, fontManager, fontShader, "../fonts/Peaberry.bmp", "", 40.0f);
    obstacle_counter.add_component<ObstacleCounterComponent>(
        &obstacle_counter, 
        (std::vector<SceneObject*>){&box1, &box2, &box3, &box4, &box5, &box6, &sphere1, &sphere2, &sphere3, &sphere4}, 
        obstacle_counter.get_component<FontComponent>()
    );

    obstacle_counter.transform.set_world_position(glm::vec3(0.0f, 1024.0f - 40.0f, 0.0f));
    obstacle_counter.transform.update_transform();

    speedoMeter.add_component<SpriteComponent>(&speedoMeter, spriteShader, txManager.load_resource("../textures/speedo_meter.png"), 256);
    speedoMeter.add_component<SpeedoMeterComponent>(&speedoMeter, 90.0f, -90.0f, speedGaugePivot.transform);
    speedoMeter.transform.set_parent(&ui_root.transform, false);
    speedoMeter.transform.update_transform();
    speedoMeter.transform.set_world_position(glm::vec3(128.0f, 128.0f, 0.1f));
    
    speedGaugePivot.transform.set_parent(&speedoMeter.transform, false);
    speedGaugePivot.transform.set_world_position(glm::vec3(0.0f, 0.0f, 0.2f));
    speedGaugePivot.transform.set_world_euler_rotation(glm::vec3(0.0f, 0.0f, 125.0f));
    
    speedGauge.add_component<SpriteComponent>(&speedGauge, spriteShader, txManager.load_resource("../textures/speed_gauge.png"), 128);
    speedGauge.transform.set_parent(&speedGaugePivot.transform, false);
    speedGauge.transform.set_world_position(glm::vec3(0.0f, 45.0f, 0.3f));

#pragma endregion

#pragma region Rendering
    renderEngine.set_scene_root(&root);
    renderEngine.set_ui_root(&ui_root);
    renderEngine.set_ui_resolution(1024, 1024);
    renderEngine.set_main_camera(&cam.transform);
    renderEngine.register_light(mainLight);
    renderEngine.register_light(secondary_light);
    renderEngine.register_light(firetruck_headlight);
    renderEngine.set_skybox_texture(skyboxTx);
#pragma endregion

    float old_time = 0.0f;
    float dt = 0.0f;
    double last_toggled_light = glfwGetTime();
    // Main loop!
    while (!glfwWindowShouldClose(window))
    {
        dt = glfwGetTime() - old_time;

        old_time = glfwGetTime();

        glClearColor(0.106f, 0.118f, 0.169f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Está sucio pero bueno mano funciona. Es pa prender y apagar la luz ambiental/la del camión
        InputManager *input;
        input = input->get_instance();
        if (input->key_is_pressed(GLFW_KEY_P) && glfwGetTime() - last_toggled_light > 0.5) {
            if (mainLight.active) {
                mainLight.disable_light();
                firetruck_headlight.enable_light();
            } else {
                mainLight.enable_light();
                firetruck_headlight.disable_light();
            }
            last_toggled_light = glfwGetTime();
        }

        sphere1.transform.set_world_euler_rotation(glm::vec3(0.0f, glfwGetTime() * 20.0f, 0.0f));
        root.transform.update_transform();
        ui_root.transform.update_transform();
        logicEngine.update(dt);
        physicsEngine.simulate();
        renderEngine.render();

        glfwPollEvents();

        if (input_mgr->key_is_pressed(GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(window, true);
            break;
        }
    }

    glfwTerminate();
    return 0;
}

void windowResizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    re_ptr->set_resolution(width, height);
}