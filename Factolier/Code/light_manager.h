#pragma once

#include <vector>
#include "light.h"
#include "utility.h"


enum class Lights
{
    DEFAULT = 0,
};


class Light_Manager
{
public:

    static Light_Manager& instance()
    {
        static Light_Manager instance;
        return instance;
    }

    Light* get_light(Lights num) { return lights[CAST_I(num)]; }

private:
    std::vector<Light*> lights;

    Light_Manager();
    ~Light_Manager();
    
};
