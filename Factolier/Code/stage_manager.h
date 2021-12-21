#pragma once


#include <memory>

class Stage_Manager final
{
private:
    Stage_Manager() = default;
    ~Stage_Manager() = default;

public:
    static Stage_Manager& instance()
    {
        static Stage_Manager instance_;
        return instance_;
    }

    void render();
};