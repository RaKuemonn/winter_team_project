
#include "scene_title.h"

#include "scene_manager.h"


void Scene_Title::initialize()
{
    title_back = std::make_unique<Sprite_Batch>(parent->device(), "./Data/cyberpunk.jpg", 1000);

    sound = std::make_unique<Sound>(parent->sound_manager()->load_sound(L"./Data/_.wav"));

    sound->play(false);

    sky_box = std::make_unique<Sky_Box>(parent->device(), L"./Data/cubemap_batch.dds");
}


void Scene_Title::uninitialize()
{

}


void Scene_Title::update(float elapsedTime)
{
    sound->set_emitter({ 0.0f, 0.0f, 0.0f });
    parent->sound_manager()->set_listener(0, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });

    sound->apply_3d(parent->sound_manager()->get_listener(0));

    sound->update();
}


void Scene_Title::render(float elapsedTime)
{
    ID3D11DeviceContext* device_context_ = parent->device_context();

    parent->state_manager()->setSS(SS::POINT);
    parent->state_manager()->setSS(SS::LINEAR);
    parent->state_manager()->setSS(SS::ANISOTROPIC);

    parent->state_manager()->setDS(DS::ON_ON);

    parent->state_manager()->setBS(BS::ALPHA);

    parent->state_manager()->setRS(RS::SOLID_NONE);


    title_back->begin(device_context_);

    for (int i = 0; i < 1; i++)
    {
        title_back->render(device_context_, 0, 0, 2, 2, 0, 0, 616, 353, 0, 0, 1, 1, 1, 1, 0);
    }

    title_back->end(device_context_);
}
