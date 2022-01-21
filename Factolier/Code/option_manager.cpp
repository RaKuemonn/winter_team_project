
#include "option_manager.h"

Option_Manager::Option_Manager(ID3D11Device* device, ID3D11DeviceContext* context)
{
    immediate_context = context;
    back = make_unique<Sprite>(device, "Data/back.png");
    bgm = make_unique<Sprite>(device, "Data/‰¹—Ê_bgm‰¼.png");
    se = make_unique<Sprite>(device, "Data/‰¹—Ê_se‰¼.png");
    meter_back = make_unique<Sprite>(device, "Data/option_1.png");
}


void Option_Manager::update()
{
    
}


void Option_Manager::render()
{
    back->render(
        immediate_context,
        300, 180,  //position
        1, 1,     // scal
        1280, 720,    // ‚Ç‚ê‚­‚ç‚¢•`‰æ‚·‚é‚©
        1280, 720,   // size
        0, 0,         // pibot
        1, 1, 1, 0.5,   // rgba
        0); // angle

    bgm->render(
        immediate_context,
        300, 180,  //position
        1, 1,     // scal
        256, 128,    // ‚Ç‚ê‚­‚ç‚¢•`‰æ‚·‚é‚©
        256, 128,   // size
        0, 0,         // pibot
        1, 1, 1, 1.0,   // rgba
        0); // angle
}
