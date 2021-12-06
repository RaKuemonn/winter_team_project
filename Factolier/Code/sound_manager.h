#pragma once

#include <string>
#include <map>
#include <memory>
#include "sound.h"


const int LISTENER_MAX = 4;         //���X�i�[�̍ő吔

class Sound_Manager
{
public:
    Sound_Manager();
    ~Sound_Manager() { audio_engine.reset(); }

public:
    //�I�[�f�B�I�G���W���̃��Z�b�g
    bool reset() { return audio_engine->Reset(); }


    //�I�[�f�B�I�G���W���̍X�V
    void update() { audio_engine->Update(); }


    //���y�Ǎ�
    std::shared_ptr<DirectX::SoundEffect> load_sound(const wchar_t* filename);


    //���o�[�u�ݒ�
    void set_reverb(DirectX::AUDIO_ENGINE_REVERB reverb) { audio_engine->SetReverb(reverb); }


    //���X�i�[�ݒ�
    void set_listener(int listener_index, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 forward, DirectX::XMFLOAT3 up = { 0.0f, 1.0f, 0.0f });


    //���X�i�[�擾
    DirectX::AudioListener get_listener(int listener_index) { return listener[listener_index]; }


    bool is_audiodevice_present() { return audio_engine->IsAudioDevicePresent(); }
    bool is_critical_error() { return audio_engine->IsCriticalError(); }

private:
    std::unique_ptr<DirectX::AudioEngine>			audio_engine = nullptr;
    using Sound_Map = std::map<std::wstring, std::shared_ptr<DirectX::SoundEffect>>;

    Sound_Map                                       resources;


    DirectX::AudioListener                          listener[LISTENER_MAX];
};
