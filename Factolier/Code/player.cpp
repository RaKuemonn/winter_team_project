#include "player.h"
#include "scene_manager.h"
#include "camera.h"
#include "input_manager.h"

Player::Player(Scene_Manager* sm)
{
    scale = DirectX::XMFLOAT3{0.01f, 0.01f, 0.01f};
    speed.x = 0.5f;
    // parent‚Ìİ’è
    set_parent(sm);
    // player“Ç‚İ‚İ
    model = std::make_unique<Model>(parent->model_manager()->load_model("./Data/Jummo/Jummo.fbx"));
}


void Player::update(float elapsedTime)
{
    updatetransfrom();
    move(elapsedTime);
}

void Player::move(float elapsedTime)
{
    
    model->play_animation(elapsedTime, 0);
    if(parent->input_manager()->STATE(0)& PAD_TRG1)
    {
        position.x += speed.x * elapsedTime;
    }
}



void Player::render()
{
     ID3D11DeviceContext* device_context_ = parent->device_context();

     parent->state_manager()->setSS(SS::POINT);
     parent->state_manager()->setSS(SS::LINEAR);
     parent->state_manager()->setSS(SS::ANISOTROPIC);

     parent->state_manager()->setDS(DS::OFF_OFF);

     parent->state_manager()->setBS(BS::ALPHA);

     parent->state_manager()->setRS(RS::SOLID_NONE);


     Shader* shader = parent->shader_manager()->get_shader(1);

  
     parent->state_manager()->setDS(DS::ON_ON);

     shader = parent->shader_manager()->get_shader(0);

     shader->begin(parent->device_context());

     model->render(parent->device_context(), world, { 1.0f, 1.0f, 1.0f, 1.0f });

     shader->end(parent->device_context());

}