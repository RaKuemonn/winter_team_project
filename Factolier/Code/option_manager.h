#pragma once

#include <d3d11.h>
#include "memory"
#include "sprite.h"
#include "binary_loader.h"



enum class Stage_Select
{
    STAGE_1 = 0,
    STAGE_2,
    STAGE_3,
    STAGE_4,
    STAGE_BOSS
};


class Option_Manager
{
public:
    Option_Manager(ID3D11Device* device, ID3D11DeviceContext* context);
    ~Option_Manager() { File_IO::write("save.dat", binary_data); }

    void update();
    void render();

    void set_next_stage(Stage_Select next_stage) { now_stage = next_stage; }
    Stage_Select get_now_stage() { return now_stage; }
    Binary_Data& get_binary() { return binary_data; }

private:
    ID3D11DeviceContext* immediate_context;

    std::unique_ptr<Sprite> back = nullptr;
    //std::unique_ptr<Sprite> option_1 = nullptr;

    Stage_Select now_stage = Stage_Select::STAGE_1;
    Binary_Data binary_data {};
};
