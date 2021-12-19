
#include "scene_title.h"
#include "scene_manager.h"
#include "camera.h"

void Scene_Title::initialize()
{
    //title_back = std::make_unique<Sprite_Batch>(parent->device(), "./Data/cyberpunk.jpg", 1000);

    //sound = std::make_unique<Sound>(parent->sound_manager()->load_sound(L"./Data/_.wav"));

    //sound->play(false);

    sky_box = std::make_unique<Sky_Box>(parent->device(), L"./Data/cubemap_batch.dds");

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

    test_model = std::make_unique<Model>(parent->model_manager()->load_model("./Data/test_cube.fbx"));
    //test_model->append_animation("./Data/Animations/Idle.fbx");
    //test_model = std::make_unique<Model>(parent->model_manager()->load_model("./Data/green.fbx"));

}


void Scene_Title::uninitialize()
{

}


void Scene_Title::update(float elapsedTime)
{
    camera_controller->update(elapsedTime, parent->input_manager());

    //entity_manager->update(elapsed_time);

}


void Scene_Title::render(float elapsedTime)
{
    ID3D11DeviceContext* device_context_ = parent->device_context();

    parent->state_manager()->setSS(SS::POINT);
    parent->state_manager()->setSS(SS::LINEAR);
    parent->state_manager()->setSS(SS::ANISOTROPIC);

    parent->state_manager()->setDS(DS::OFF_OFF);

    parent->state_manager()->setBS(BS::ALPHA);

    parent->state_manager()->setRS(RS::SOLID_NONE);


    Shader* shader = parent->shader_manager()->get_shader(Shaders::SKY);
    
    shader->begin(parent->device_context());
    
    sky_box->render(parent->device_context()); // ˆê”Ôæ‚É•`‰æ‚³‚¹‚é
    
    shader->end(parent->device_context());


    //title_back->begin(device_context_);
    //
    //for (int i = 0; i < 1; i++)
    //{
    //    title_back->render(device_context_, 0, 0, 2, 2, 0, 0, 616, 353, 0, 0, 1, 1, 1, 1, 0);
    //}
    //
    //title_back->end(device_context_);

    DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f);

    DirectX::XMFLOAT4X4 world = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    //DirectX::XMMATRIX S = DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f);
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(0.1f, 0.15f, 0.1f);
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(0, 0, 0);
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(0, 0, 0);
    DirectX::CXMMATRIX W = S * R * T;
    DirectX::XMStoreFloat4x4(&world, W);

    parent->state_manager()->setDS(DS::ON_ON);

    shader = parent->shader_manager()->get_shader(Shaders::OCEAN);

    shader->begin(parent->device_context(), elapsedTime * 0.5f);

    test_model->render(parent->device_context(), world, { 1.0f, 1.0f, 1.0f, 0.5f });
    //test_model->render_mesh(parent->device_context(), world, { 1.0f, 1.0f, 1.0f, 1.0f }, 0);

    shader->end(parent->device_context());

    //DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f);
    //
    //DirectX::XMFLOAT4X4 world = {
    //    1.0f, 0.0f, 0.0f, 0.0f,
    //    0.0f, 1.0f, 0.0f, 0.0f,
    //    0.0f, 0.0f, 1.0f, 0.0f,
    //    0.0f, 0.0f, 0.0f, 1.0f
    //};
    //DirectX::XMMATRIX S = DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f);
    //DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(0, 0, 0);
    //DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(0, 0, 0);
    //DirectX::CXMMATRIX W = S * R * T;
    //DirectX::XMStoreFloat4x4(&world, W);
    //
    //DirectX::XMMATRIX World = DirectX::XMLoadFloat4x4(&world);
    //World = scale;
    //DirectX::XMStoreFloat4x4(&world, World);
    //
    //parent->state_manager()->setDS(DS::ON_ON);
    //
    //shader = parent->shader_manager()->get_shader(0);
    //
    //shader->begin(parent->device_context());
    //
    //test_model->render(parent->device_context(), world, { 1.0f, 1.0f, 1.0f, 1.0f });
    //
    //shader->end(parent->device_context());
    //player->render();
}
