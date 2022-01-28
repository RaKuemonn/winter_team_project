#pragma once

#include "decorator.h"
#include "scene_manager.h"
#include "transform.h"

class Deco_Egg final : public Decorator
{
public:
    Deco_Egg(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& position)
    {
        load_model(ptr_scene_manager_->model_manager()->load_model(Model_Paths::Deco::egg, true));
        get_transform()->set_position(position);
        y_radian += 1.2f;
        get_transform()->set_euler({ 0.0f,y_radian,0.0f });
        get_transform()->Update();
    }
};

class Deco_Egg2 final : public Decorator
{
public:
    Deco_Egg2(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& position)
    {
        load_model(ptr_scene_manager_->model_manager()->load_model(Model_Paths::Deco::egg_2, true));
        get_transform()->set_position(position);
        y_radian += 1.2f;
        get_transform()->set_euler({ 0.0f,y_radian,0.0f });
        get_transform()->Update();
    }
};