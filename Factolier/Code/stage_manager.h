#pragma once

#include <memory>
#include <vector>
class Stage;


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

    void spawn_register(std::unique_ptr<Stage>& stage);
    void spawn_register(std::unique_ptr<Stage>&& stage);
    void spawn_register(std::shared_ptr<Stage>& stage);
    void remove_register(Stage* stage);

    void all_clear();



private:
    std::vector<std::shared_ptr<Stage>> vec_stages = {};
    std::vector<std::shared_ptr<Stage>> vec_registers = {};
    std::vector<std::shared_ptr<Stage>> vec_removes = {};
};