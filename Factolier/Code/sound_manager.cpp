
#include "sound_manager.h"


using DirectX::AUDIO_ENGINE_FLAGS;
using DirectX::AudioEngine;
using DirectX::SoundEffect;
using DirectX::WaveBank;

using DirectX::AudioEngine_Default;
using DirectX::AudioEngine_EnvironmentalReverb;
using DirectX::AudioEngine_ReverbUseFilters;
using DirectX::AudioEngine_Debug;


//コンストラクタ
Sound_Manager::Sound_Manager()
{
    // Create DirectXTK for Audio objects
    AUDIO_ENGINE_FLAGS eflags = AudioEngine_EnvironmentalReverb | AudioEngine_ReverbUseFilters;
#ifdef _DEBUG
    eflags = eflags | AudioEngine_Debug;
#endif
    //audio_engine.reset(new AudioEngine(eflags));
    //            for (auto &p : sound_volume) p = 1.0f;TODO

    audio_engine = std::make_unique<AudioEngine>(eflags);
    audio_engine->SetReverb(DirectX::Reverb_Off);
}


//音楽読込
std::shared_ptr<DirectX::SoundEffect> Sound_Manager::load_sound(const wchar_t* filename)
{
    //リソースを検索
    std::shared_ptr<DirectX::SoundEffect> resource;

    auto it = resources.find(filename);

    if (it != resources.end())
    {
        resource = it->second;

        return resource;
    }

    resource = std::make_shared<DirectX::SoundEffect>(audio_engine.get(), filename);

    resources.emplace(filename, resource);

    return resource;
}


//リスナー設定
void Sound_Manager::set_listener(const int listener_index, const DirectX::XMFLOAT3 pos, const DirectX::XMFLOAT3 forward, const DirectX::XMFLOAT3 up)
{
    listener[listener_index].SetPosition(pos);
    listener[listener_index].SetOrientation(forward, up);
}

