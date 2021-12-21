
#include "light_manager.h"


Light_Manager::Light_Manager()
{
    lights.emplace_back(new Light());
}


Light_Manager::~Light_Manager()
{
    for (Light* s : lights)
    {
        safe_delete(s);
    }

    lights.clear();
}

