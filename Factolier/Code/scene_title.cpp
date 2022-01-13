
#include "scene_title.h"
#include "scene_manager.h"
#include "camera.h"
#include "scene_loading.h"
#include "scene_game.h"

void Scene_Title::initialize(Scene_Manager* parent_)
{
    parent = parent_;

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
    camera.set_perspective_fov(DirectX::XMConvertToRadians(70),
        1280 / 720,
        0.1f,
        100.0f);


    camera_controller = make_unique<Camera_Controller>();

    DirectX::XMFLOAT3 target = { 0.0f, 0.0f, 0.0f };
    camera_controller->set_target(target);

    //test_model = std::make_unique<Model>(parent->model_manager()->load_model("./Data/test_tree_winter.fbx"));
   stage_model = std::make_unique<Model>(parent->model_manager()->load_model("./Data/stage_demo.fbx"));
    //test_model->append_animation("./Data/Animations/Idle.fbx");
    //test_model = std::make_unique<Model>(parent->model_manager()->load_model("./Data/green.fbx"));

    test_effect = std::make_unique<Effect>(parent->effect_manager(), parent->mutex(), "./Data/Effect/dummy.efk");

    test_effect->play({ 0.0f, 0.0f, 0.0 }, parent->effect_manager());

}


void Scene_Title::uninitialize()
{

}


void Scene_Title::update(float elapsed_time)
{
    camera_controller->update(elapsed_time, parent->input_manager());

    //test_model->play_animation(elapsed_time, 1);

    parent->effect_manager()->update(elapsed_time);

    //parent->change_scene(new Scene_Loading(new Scene_Game()));
}


void Scene_Title::render(float elapsed_time)
{
    parent->state_manager()->setSS(SS::POINT);
    parent->state_manager()->setSS(SS::LINEAR);
    parent->state_manager()->setSS(SS::ANISOTROPIC);

    parent->state_manager()->setDS(DS::ON_ON);

    parent->state_manager()->setBS(BS::COVERAGE);

    parent->state_manager()->setRS(RS::SOLID_BACK);


    //行列を作成
    DirectX::XMFLOAT4X4 world = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    DirectX::XMFLOAT4X4 world_stage = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    };

    {
        DirectX::XMMATRIX S = DirectX::XMMatrixScaling(0.005f, 0.005f, 0.005f);
        //DirectX::XMMATRIX S = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
        //DirectX::XMMATRIX S = DirectX::XMMatrixScaling(1.0f, 0.15f, 1.0f);
        DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(0), 0, DirectX::XMConvertToRadians(0));
        DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(0, 0, 0);
        DirectX::XMMATRIX W = S * R * T;

        XMFLOAT4X4 coordinate_system_transforms = { -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };	// 0:RHS Y-UP
        XMMATRIX CW = XMLoadFloat4x4(&coordinate_system_transforms);
        W *= CW;
        DirectX::XMStoreFloat4x4(&world, W);

        
        DirectX::XMMATRIX S_s = DirectX::XMMatrixScaling(0.1f, 0.1f, 0.1f);
        //DirectX::XMMATRIX S = DirectX::XMMatrixScaling(1.0f, 0.15f, 1.0f);
        DirectX::XMMATRIX R_s = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(0), 0, 0);
        DirectX::XMMATRIX T_s = DirectX::XMMatrixTranslation(0, 0, 0);
        DirectX::XMMATRIX W_s = S_s * R_s * T_s;

        W_s *= CW;
        DirectX::XMStoreFloat4x4(&world_stage, W_s);
    }



    Shader* shader = nullptr;

    //シャドウマップ生成
    {
        shader = parent->shader_manager()->get_shader(Shaders::SHADOW);

        shader->begin(parent->device_context(), elapsed_time * 0.1f);


        //stage_model->render(parent->device_context(), world, { 1.0f, 1.0f, 1.0f, 1.0f });
        //test_model->render(parent->device_context(), world, { 1.0f, 1.0f, 1.0f, 1.0f });

        shader->end(parent->device_context());
    }


    //レンダーターゲットビューと深度ステンシルビューを元に戻す
    {
        ID3D11RenderTargetView* rtv = parent->render_target_view();
        ID3D11DepthStencilView* dsv = parent->depth_stencil_view();
        FLOAT color[]{ 1.0f, 1.0f, 1.0f, 1.0f };

        parent->device_context()->ClearRenderTargetView(rtv, color);
        parent->device_context()->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        parent->device_context()->OMSetRenderTargets(1, &rtv, dsv);
    }


    parent->state_manager()->setDS(DS::OFF_OFF);

    //スカイボックス描画
    {
        shader = parent->shader_manager()->get_shader(Shaders::SKY);

        shader->begin(parent->device_context());

        sky_box->render(parent->device_context());

        shader->end(parent->device_context());
    }


    parent->state_manager()->setDS(DS::ON_ON);

    //エンティティ描画
    {
        shader = parent->shader_manager()->get_shader(Shaders::PHONG);

        shader->begin(parent->device_context(), elapsed_time * 0.1f);

        stage_model->render(parent->device_context(), world_stage, { 1.0f, 1.0f, 1.0f, 1.0f });

        parent->state_manager()->setRS(RS::SOLID_NONE);
       // test_model->render(parent->device_context(), world, { 1.0f, 1.0f, 1.0f, 1.0f });

        shader->end(parent->device_context());
    }


    //エフェクト描画
    {
        parent->effect_manager()->render(Camera::Instance().get_view(), Camera::Instance().get_projection());
    }

}
