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
        get_transform()->Update();
    }
};