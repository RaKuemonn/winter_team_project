#pragma once

#include <string>
#include <map>
#include <memory>
#include "sound.h"


const int LISTENER_MAX = 4;         //リスナーの最大数

class Sound_Manager
{
public:
    Sound_Manager();
    ~Sound_Manager() { audio_engine.reset(); }

public:
    //オーディオエンジンのリセット
    bool reset() { return audio_engine->Reset(); }


    //オーディオエンジンの更新
    void update() { audio_engine->Update(); }


    //音楽読込
    std::shared_ptr<DirectX::SoundEffect> load_sound(const wchar_t* filename);


    //リバーブ設定
    void set_reverb(DirectX::AUDIO_ENGINE_REVERB reverb) { audio_engine->SetReverb(reverb); }


    //リスナー設定
    void set_listener(int listener_index, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 forward, DirectX::XMFLOAT3 up = { 0.0f, 1.0f, 0.0f });


    //リスナー取得
    DirectX::AudioListener get_listener(int listener_index) { return listener[listener_index]; }


    bool is_audiodevice_present() { return audio_engine->IsAudioDevicePresent(); }
    bool is_critical_error() { return audio_engine->IsCriticalError(); }

private:
    std::unique_ptr<DirectX::AudioEngine>			audio_engine = nullptr;
    using Sound_Map = std::map<std::wstring, std::shared_ptr<DirectX::SoundEffect>>;

    Sound_Map                                       resources;


    DirectX::AudioListener                          listener[LISTENER_MAX];
};
