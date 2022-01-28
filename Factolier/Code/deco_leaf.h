#pragma once

#include "decorator.h"
#include "scene_manager.h"
#include "transform.h"

class Deco_Leaf_Fall final : public Decorator
{
public:
    Deco_Leaf_Fall(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& position)
    {
        load_model(ptr_scene_manager_->model_manager()->load_model(Model_Paths::Deco::aki_leaf, true));
        get_transform()->set_position(position);
        get_transform()->set_scale(stage::scale * 0.3f);
        get_transform()->Update();
    }
};

class Deco_Leaf_Winter final : public Decorator
{
public:
    Deco_Leaf_Winter(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& position)
    {
        load_model(ptr_scene_manager_->model_manager()->load_model(Model_Paths::Deco::fuyu_leaf, true));
        get_transform()->set_position(position);
        get_transform()->set_scale(stage::scale * 0.3f);
        get_transform()->Update();
    }
};



class Deco_Wood_Spring final : public Decorator
{
public:
    Deco_Wood_Spring(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& position)
    {
        load_model(ptr_scene_manager_->model_manager()->load_model(Model_Paths::Deco::wood_spring, true));
        get_transform()->set_position(position);
        y_radian += 1.2f;
        get_transform()->set_euler({ 0.0f,y_radian,0.0f });
        get_transform()->Update();
    }
};

class Deco_Wood_Summer final : public Decorator
{
public:
    Deco_Wood_Summer(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& position)
    {
        load_model(ptr_scene_manager_->model_manager()->load_model(Model_Paths::Deco::wood_summer, true));
        get_transform()->set_position(position);
        y_radian += 1.2f;
        get_transform()->set_euler({ 0.0f,y_radian,0.0f });
        get_transform()->Update();
    }
};

class Deco_Wood_Fall final : public Decorator
{
public:
    Deco_Wood_Fall(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& position)
    {
        load_model(ptr_scene_manager_->model_manager()->load_model(Model_Paths::Deco::wood_fall, true));
        get_transform()->set_position(position);
        y_radian += 1.2f;
        get_transform()->set_euler({ 0.0f,y_radian,0.0f });
        get_transform()->Update();
    }
};

class Deco_Wood_Winter final : public Decorator
{
public:
    Deco_Wood_Winter(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& position)
    {
        load_model(ptr_scene_manager_->model_manager()->load_model(Model_Paths::Deco::wood_winter, true));
        get_transform()->set_position(position);
        y_radian += 1.2f;
        get_transform()->set_euler({ 0.0f,y_radian,0.0f });
        get_transform()->Update();
    }
};