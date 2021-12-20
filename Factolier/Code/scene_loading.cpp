
#include <thread>
#include "scene_loading.h"
#include "scene_manager.h"


void Scene_Loading::initialize()
{
    //スレッド開始
    std::thread thread(loading_thread, this);

    //スレッドの管理を放棄
    thread.detach();
}


void Scene_Loading::uninitialize()
{
    
}


void Scene_Loading::update(float elapsed_time)
{
    //次のシーンの準備が完了したらシーンを切り替える
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
    //COM関連の初期化でスレッド毎に呼ぶ必要がある
    CoInitialize(nullptr);

    //次のシーンの初期化を行う
    scene->next_scene->initialize();

    //スレッドが終わる前にCOM関連の終了化
    CoUninitialize();

    //次のシーンの準備完了設定
    scene->next_scene->set_ready();
}