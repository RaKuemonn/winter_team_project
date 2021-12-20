
#include <thread>
#include "scene_loading.h"
#include "scene_manager.h"


void Scene_Loading::initialize()
{
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
    //���̃V�[���̏���������������V�[����؂�ւ���
    if (next_scene->is_ready())
    {
        parent->change_scene(next_scene);
    }
}


void Scene_Loading::render(float elapsed_time)
{
    
}



void Scene_Loading::loading_thread(Scene_Loading* scene)
{
    //COM�֘A�̏������ŃX���b�h���ɌĂԕK�v������
    CoInitialize(nullptr);

    //���̃V�[���̏��������s��
    scene->next_scene->initialize();

    //�X���b�h���I���O��COM�֘A�̏I����
    CoUninitialize();

    //���̃V�[���̏��������ݒ�
    scene->next_scene->set_ready();
}