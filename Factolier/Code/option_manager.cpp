
#include "option_manager.h"
#include "imgui.h"

Option_Manager::Option_Manager(ID3D11Device* device, ID3D11DeviceContext* context)
{
    immediate_context = context;
    back = make_unique<Sprite>(device, "Data/back.png");
    File_IO::open("save.dat", binary_data);
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
        1280, 720,    // どれくらい描画するか
        1280, 720,   // size
        0, 0,         // pibot
        1, 1, 1, 0.5,   // rgba
        0); // angle
}
