#pragma once

#include <functional>
#include "option_manager.h"

// Stage_ManagerとEntity_Managerのインスタンスの
// 当たり判定処理を受け持つクラス

class Collision_Manager final
{
public:
    Collision_Manager(Stage_Select stage_);
    ~Collision_Manager() = default;

    void judge(const float elapsed_time);

private:
    std::function<void(const std::tuple<std::vector<short>, std::vector<short>, std::vector<short>, std::vector<short>>&)> water_movement_function;
    std::function<void(const float ,const std::tuple<std::vector<short>, std::vector<short>, std::vector<short>, std::vector<short>>&)> judges_function;
};
