#pragma once

#include <mutex>
#include "memory"
#include "state_manager.h"
#include "shader_manager.h"
#include "model_manager.h"
#include "sound_manager.h"
#include "input_manager.h"
#include "effect_manager.h"
#include "option_manager.h"
#include "scene.h"


class Scene_Manager
{
public:
    void initialize(ID3D11Device* device, ID3D11DeviceContext* context, ID3D11RenderTargetView* render, ID3D11DepthStencilView* depth, HRESULT hr);
    void uninitialize();

    void update(float elapsed_time,const HWND& hwnd_);
    void render(float elapsed_time);

    void change_scene(Scene* next_scene);
    void clear();
      
private:
    ID3D11Device* device_ = nullptr;
    ID3D11DeviceContext* immediate_context_ = nullptr;
    ID3D11RenderTargetView* render_target_view_ = nullptr;
    ID3D11DepthStencilView* depth_stencil_view_ = nullptr;

    std::mutex mutex_;

    std::unique_ptr<State_Manager>              state_manager_ = nullptr;
    std::unique_ptr<Shader_Manager>             shader_manager_ = nullptr;
    std::unique_ptr<Model_Manager>              model_manager_ = nullptr;
    std::unique_ptr<Sound_Manager>              sound_manager_ = nullptr;
    std::unique_ptr<Input_Manager_Interface>    input_manager_ = nullptr;
    std::unique_ptr<Effect_Manager>             effect_manager_ = nullptr;
    std::unique_ptr<Option_Manager>             option_manager_ = nullptr;

    Scene* scene = nullptr;

public:
    State_Manager*      state_manager() { return state_manager_.get(); }
    Shader_Manager*     shader_manager(){ return shader_manager_.get(); }
    Model_Manager*      model_manager() { return model_manager_.get(); }
    Sound_Manager*      sound_manager() { return sound_manager_.get(); }
    Input_Manager*      input_manager() { return input_manager_.get(); }
    Effect_Manager*     effect_manager(){ return effect_manager_.get(); }
    Option_Manager*     option_manager(){ return option_manager_.get(); }

    ID3D11Device* device() { return device_; }
    ID3D11DeviceContext* device_context() { return immediate_context_; }
    ID3D11DepthStencilView* depth_stencil_view() { return depth_stencil_view_; }
    ID3D11RenderTargetView* render_target_view() { return render_target_view_; }

    std::mutex& mutex() { return mutex_; }

};