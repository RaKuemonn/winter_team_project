
#include <thread>
#include "scene_loading.h"
#include "scene_manager.h"


void Scene_Loading::initialize(Scene_Manager* parent_)
{
    parent = parent_;

    icon = make_unique<Sprite>(parent->device(), "./Data/Sprite/���[�h�A�C�R��.png");
    //�X���b�h�J�n
    std::thread thread(loading_thread, this);

    //�X���b�h�̊Ǘ������
    thread.detach();
}


void Scene_Loading::uninitialize()
{
    
}


void Scene_Loading::update(float elapsed_time)
{
    angle += elapsed_time * 20;
    //���̃V�[���̏���������������V�[����؂�ւ���
    if (next_scene->is_ready())
    {
        parent->change_scene(next_scene);
    }
}


void Scene_Loading::render(float elapsed_time)
{
    ID3D11DeviceContext* device_context_ = parent->device_context();
    ID3D11RenderTargetView* rtv = parent->render_target_view();
    ID3D11DepthStencilView* dsv = parent->depth_stencil_view();
    FLOAT color[]{ 0.0f, 0.0f, 0.0f, 1.0f };

    parent->device_context()->ClearRenderTargetView(rtv, color);
    parent->device_context()->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    parent->device_context()->OMSetRenderTargets(1, &rtv, dsv);

    icon->render(device_context_,
        960, 540,  //position
        1.0f, 1.0f,     // scal
        200, 200,    // �ǂꂭ�炢�`�悷�邩
        200, 200,   // size
        0.5, 0.5,         // pibot
        1, 1, 1, 1,   // rgba
        angle);
 

}



void Scene_Loading::loading_thread(Scene_Loading* scene)
{
    //COM�֘A�̏������ŃX���b�h���ɌĂԕK�v������
    HRESULT hr = CoInitialize(nullptr);

    //���̃V�[���̏��������s��
    scene->next_scene->initialize(scene->parent);

    //�X���b�h���I���O��COM�֘A�̏I����
    CoUninitialize();

    //���̃V�[���̏��������ݒ�
    scene->next_scene->set_ready();
}