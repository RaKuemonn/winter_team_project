#pragma once

#include <DirectXMath.h>
#include <d3d11.h>
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
    

    void update(const float elapsed_time);
    void render(ID3D11DeviceContext* ptr_device_context);
    bool ray_cast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, struct Hit_Result* hit_result_);
    

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