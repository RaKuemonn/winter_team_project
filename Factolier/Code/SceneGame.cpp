#include "scene_manager.h"
#include "camera.h"
#include "SceneGame.h"


void SceneGame::initialize()
{
    sky_box = std::make_unique<Sky_Box>(parent->device(), L"./Data/cubemap_batch.dds");
    //sky_box.reset(new Sky_Box(parent->device(), L"./Data/cubemap_batch.dds"));


    Camera& camera = Camera::Instance();
    camera.set_lookat(
        DirectX::XMFLOAT3(0, 10, -10),
        DirectX::XMFLOAT3(0, 0, 0),
        DirectX::XMFLOAT3(0, 1, 0)
    );
    camera.set_perspective_fov(DirectX::XMConvertToRadians(90),
        1280 / 720,
        0.1f,
        1000.0f);
    
    
    camera_controller = make_unique<Camera_Controller>();
    
    DirectX::XMFLOAT3 target = { 0.0f, 0.0f, 0.0f };
    camera_controller->set_target(target);
    
    
    player = std::make_unique<Player>(parent);
    //test_model = std::make_unique<Model>(parent->model_manager()->load_model("./Data/green.fbx"));
    //player = std::make_unique<Model>(parent->model_manager()->load_model("./Data/Mr.Incredible/Mr.Incredible.fbx"));

}


void SceneGame::uninitialize()
{

}


void SceneGame::update(float elapsedTime)
{
    camera_controller->update(elapsedTime, parent->input_manager());
   
    player->update(elapsedTime);
}


void SceneGame::render(float elapsedTime)
{
    ID3D11DeviceContext* device_context_ = parent->device_context();

    parent->state_manager()->setSS(SS::POINT);
    parent->state_manager()->setSS(SS::LINEAR);
    parent->state_manager()->setSS(SS::ANISOTROPIC);

    parent->state_manager()->setDS(DS::OFF_OFF);

    parent->state_manager()->setBS(BS::ALPHA);

    parent->state_manager()->setRS(RS::SOLID_NONE);


    Shader* shader = parent->shader_manager()->get_shader(1);

    shader->begin(parent->device_context());

    sky_box->render(parent->device_context()); // ˆê”Ôæ‚É•`‰æ‚³‚¹‚é

    shader->end(parent->device_context());



    DirectX::XMFLOAT4X4 world = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
   

    parent->state_manager()->setDS(DS::ON_ON);

    shader = parent->shader_manager()->get_shader(0);

    shader->begin(parent->device_context());

    //test_model->render(parent->device_context(), world, { 1.0f, 1.0f, 1.0f, 1.0f });
    //player->render(parent->device_context(), world, { 1.0f, 1.0f, 1.0f, 1.0f });

    // player•`‰æ
    player->render();
    

    shader->end(parent->device_context());

}