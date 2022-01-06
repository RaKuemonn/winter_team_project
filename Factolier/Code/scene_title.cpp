
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

    test_model = std::make_unique<Model>(parent->model_manager()->load_model("./Data/nico.fbx"));
    stage_model = std::make_unique<Model>(parent->model_manager()->load_model("./Data/stage_demo.fbx"));
    //test_model->append_animation("./Data/Animations/Idle.fbx");
    //test_model = std::make_unique<Model>(parent->model_manager()->load_model("./Data/green.fbx"));

}


void Scene_Title::uninitialize()
{

}


void Scene_Title::update(float elapsed_time)
{
    camera_controller->update(elapsed_time, parent->input_manager());

    test_model->play_animation(elapsed_time, 1);

}


void Scene_Title::render(float elapsed_time)
{
    parent->state_manager()->setSS(SS::POINT);
    parent->state_manager()->setSS(SS::LINEAR);
    parent->state_manager()->setSS(SS::ANISOTROPIC);

    parent->state_manager()->setDS(DS::ON_ON);

    parent->state_manager()->setBS(BS::ALPHA);

    parent->state_manager()->setRS(RS::SOLID_BACK);

    DirectX::XMFLOAT4X4 world = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f);
    //DirectX::XMMATRIX S = DirectX::XMMatrixScaling(1.0f, 0.15f, 1.0f);
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(0), 0, 0);
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(0, 0, 0);
    DirectX::XMMATRIX W = S * R * T;

    XMFLOAT4X4 coordinate_system_transforms = { -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };	// 0:RHS Y-UP
    XMMATRIX CW = XMLoadFloat4x4(&coordinate_system_transforms);
    W *= CW;
    DirectX::XMStoreFloat4x4(&world, W);


    //Shader* shader = parent->shader_manager()->get_shader(Shaders::SHADOW);
    //
    //shader->begin(parent->device_context(), false, elapsed_time * 0.1f);

    //
    ////stage_model->render(parent->device_context(), world, { 1.0f, 1.0f, 1.0f, 1.0f });
    //test_model->render(parent->device_context(), world, { 1.0f, 1.0f, 1.0f, 1.0f });
    //
    //shader->end(parent->device_context());


    Shader* shader = parent->shader_manager()->get_shader(Shaders::SHADOW);

    shader->begin(parent->device_context(), elapsed_time * 0.1f);


    //stage_model->render(parent->device_context(), world, { 1.0f, 1.0f, 1.0f, 1.0f });
    test_model->render(parent->device_context(), world, { 1.0f, 1.0f, 1.0f, 1.0f });

    shader->end(parent->device_context());



    ID3D11RenderTargetView* rtv = parent->render_target_view();
    ID3D11DepthStencilView* dsv = parent->depth_stencil_view();
    FLOAT color[]{ 1.0f, 1.0f, 1.0f, 1.0f };
    
    parent->device_context()->ClearRenderTargetView(rtv, color);
    parent->device_context()->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    parent->device_context()->OMSetRenderTargets(1, &rtv, dsv);

    parent->state_manager()->setSS(SS::POINT);
    parent->state_manager()->setSS(SS::LINEAR);
    parent->state_manager()->setSS(SS::ANISOTROPIC);

    parent->state_manager()->setDS(DS::ON_ON);

    parent->state_manager()->setBS(BS::ALPHA);

    parent->state_manager()->setRS(RS::SOLID_BACK);



    parent->state_manager()->setDS(DS::OFF_OFF);

    shader = parent->shader_manager()->get_shader(Shaders::SKY);
    
    shader->begin(parent->device_context());
    
    sky_box->render(parent->device_context());
    
    shader->end(parent->device_context());


    //title_back->begin(device_context_);
    //
    //for (int i = 0; i < 1; i++)
    //{
    //    title_back->render(device_context_, 0, 0, 2, 2, 0, 0, 616, 353, 0, 0, 1, 1, 1, 1, 0);
    //}
    //
    //title_back->end(device_context_);


    parent->state_manager()->setDS(DS::ON_ON);


    shader = parent->shader_manager()->get_shader(Shaders::PHONG);
    
    shader->begin(parent->device_context(), elapsed_time * 0.1f);
    
    stage_model->render(parent->device_context(), world, { 1.0f, 1.0f, 1.0f, 1.0f });
    test_model->render(parent->device_context(), world, { 1.0f, 1.0f, 1.0f, 1.0f });
    
    shader->end(parent->device_context());

}
