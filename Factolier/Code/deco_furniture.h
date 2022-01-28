#pragma once

#include "decorator.h"
#include "scene_manager.h"
#include "transform.h"

class Deco_Tetrapos final : public Decorator
{
public:
    Deco_Tetrapos(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& position)
    {
        load_model(ptr_scene_manager_->model_manager()->load_model(Model_Paths::Deco::tetrapod, true));
        get_transform()->set_position(position);
        get_transform()->Update();
    }
};

class Deco_Umbrella_and_Bed final : public Decorator
{
public:
    Deco_Umbrella_and_Bed(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& position)
    {
        load_model(ptr_scene_manager_->model_manager()->load_model(Model_Paths::Deco::umbrella_and_bed, true));
        get_transform()->set_position(position);
        get_transform()->set_scale(stage::scale * 0.15f);
        get_transform()->Update();
    }
};

class Deco_Kamakura final : public Decorator
{
public:
    Deco_Kamakura(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& position)
    {
        load_model(ptr_scene_manager_->model_manager()->load_model(Model_Paths::Deco::kamakura, true));
        get_transform()->set_position(position);
        get_transform()->set_euler({ 0.0f,-3.14f,0.0f });
        get_transform()->set_scale(stage::scale * 0.2f);
        get_transform()->Update();
    }
};

class Deco_Snow_Man final : public Decorator
{
public:
    Deco_Snow_Man(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& position)
    {
        load_model(ptr_scene_manager_->model_manager()->load_model(Model_Paths::Deco::snow_man, true));
        get_transform()->set_position(position);
        get_transform()->set_euler({ 0.0f,-3.14f,0.0f });
        get_transform()->set_scale(stage::scale * 0.3f);
        get_transform()->Update();
    }
};

class Deco_Pumpkin final : public Decorator
{
public:
    Deco_Pumpkin(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& position)
    {
        load_model(ptr_scene_manager_->model_manager()->load_model(Model_Paths::Deco::pumpkin, true));
        get_transform()->set_position(position);
        y_radian += 1.2f;
        get_transform()->set_euler({ 0.0f,y_radian,0.0f });
        get_transform()->Update();
    }
};

class Deco_Ball final : public Decorator
{
public:
    Deco_Ball(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& position)
    {
        load_model(ptr_scene_manager_->model_manager()->load_model(Model_Paths::Deco::ball, true));
        get_transform()->set_position(position);
        get_transform()->Update();
    }
};

class Deco_Kinoko final : public Decorator
{
public:
    Deco_Kinoko(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& position)
    {
        load_model(ptr_scene_manager_->model_manager()->load_model(Model_Paths::Deco::kinoko, true));
        get_transform()->set_position(position);
        y_radian += 1.2f;
        get_transform()->set_euler({ 0.0f,y_radian,0.0f });
        get_transform()->Update();
    }
};