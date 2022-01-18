#pragma once

#include <algorithm>
#include "Audio.h"



extern float fps;


class Sound {
private:
    std::shared_ptr<DirectX::SoundEffect>           music = nullptr;
    std::unique_ptr<DirectX::SoundEffectInstance>	music_inst = nullptr;
    DirectX::AudioEmitter                           emitter{};
    float                                           def_volume = 0.5f;
    float											cur_volume = 0.0f;
    float                                           fade_volume = 0.0f;
    float                                           add = 0.0f;
    float                                           pitch = 0.0f;
    float                                           pan = 0.0f;

public:
    //�R���X�g���N�^
    Sound(std::shared_ptr<DirectX::SoundEffect> resource, float volume = (0.5f))
    {
        if (music_inst) music_inst.reset();
        music = resource;
        def_volume = volume;
        cur_volume = volume;
    }


    //�f�X�g���N�^
    ~Sound()
    {
        unload();
    }

    
    //�X�V����
    void update()
    {
        // �|�[�Y�����~���̓t�F�[�h���s��Ȃ�
        if (is_in_use() && get_state() != DirectX::SoundState::PLAYING)
            return;

        // �t�F�[�h�C��
        if (add > 0)
        {
            cur_volume += add;
            if (cur_volume > fade_volume)
            {
                cur_volume = fade_volume;
                add = 0.0f;
            }
            //set_volume(cur_volume);
        }

        // �t�F�[�h�A�E�g
        if (add < 0)
        {
            cur_volume += add;
            if (cur_volume < fade_volume)
            {
                cur_volume = fade_volume;
                add = 0.0f;
            }
            //set_volume(cur_volume);

            // �{�����[����0�ȉ��ɂȂ������~
            if (cur_volume <= 0.0f)
            {
                stop();
            }
        }
    }


    //�j��
    void unload()
    {
        if (music_inst)  music_inst.reset();
        if (music)      music.reset();
    }

    
    //�Đ�
    void play(bool is_loop)
    {
        if (!music) return;
        music_inst = music->CreateInstance(DirectX::SoundEffectInstance_Use3D | DirectX::SoundEffectInstance_ReverbUseFilters);
        //music_inst->Apply3D()
        music_inst->Play(is_loop);
        //cur_volume = def_volume;
        //music_inst->SetVolume(cur_volume);
        //fade_volume = cur_volume;
        //add = 0.0f;
        //pitch = 0.0f;
        //pan = 0.0f;
    }

    
    //��~
    void stop()
    {
        if (!music_inst) return;
        music_inst->Stop();
    }


    //�|�[�Y
    void pause()
    {
        if (!music_inst) return;
        music_inst->Pause();
    }

    
    //���W���[���i�|�[�Y����̕��A�j
    void resume()
    {
        if (!music_inst) return;
        music_inst->Resume();
    }

    
    //�{�����[���̐ݒ�
    void set_volume(float volume)
    {
        if (!music_inst) return;
        music_inst->SetVolume(volume);
        cur_volume = volume;
    }

    
    //�{�����[���̎擾
    float get_volume()
    {
        return cur_volume;
    }


    //�t�F�[�h�C���E�t�F�[�h�A�E�g
    void fade(float fade_sec, float volume)
    {
        fade_volume = volume;
        add = (fade_volume - cur_volume) / (fps * fade_sec);   // 1�t���[��������̃{�����[���̕ψ�
    }

    
    //��Ԃ̎擾�iisInUse��true�̏ꍇ�̂ݍs�����Ɓj
    DirectX::SoundState get_state()
    {
        return music_inst->GetState();
    }

    
    //���[�v�̎擾
    bool is_looped()
    {
        return music_inst->IsLooped();
    }

    
    //�p���̐ݒ�i���E�̃o�����X�j
    void set_pan(float p)
    {
        pan = p;
        music_inst->SetPan(pan);
    }

    
    //�p���̎擾�i���E�̃o�����X�j
    float get_pan()
    {
        return pan;
    }

    
    //�p���̒���
    float adjust_pan(float p)
    {
        pan += p;
        pan = (std::max)((std::min)(pan, 1.0f), -1.0f);
        music_inst->SetPan(pan);
        return pan;
    }

    
    //�s�b�`�̐ݒ�
    void set_pitch(float p)
    {
        pitch = p;
        music_inst->SetPitch(pitch);
    }

    
    //�s�b�`�̎擾
    float get_pitch()
    {
        return pitch;
    }

    
    //�s�b�`�̒���
    float adjust_pitch(float p)
    {
        pitch += p;
        pitch = (std::max)((std::min)(pitch, 1.0f), -1.0f);
        music_inst->SetPitch(pitch);
        return pitch;
    }

    
    //�t�H�[�}�b�g�̎擾
    const WAVEFORMATEX* get_format()
    {
        return music->GetFormat();
    }

    
    //���y���g�p�\���ǂ���
    bool is_in_use()
    {
        if (!music) return false;
        return music->IsInUse();
    }


    //�G�~�b�^�[�ݒ�
    void set_emitter(const DirectX::XMFLOAT3 pos)
    {
        emitter.SetPosition(pos);
        //emitter.OrientTop = { 0.0f, 1.0f, 0.0f };
        emitter.ChannelCount = music->GetFormat()->nChannels;
        //emitter.ChannelRadius = 20.0f;
        //emitter.CurveDistanceScaler = 1.4f;
    }


    //�RD�G�t�F�N�g�K�p
    void apply_3d(const DirectX::AudioListener& listener)
    {
        using namespace DirectX;
        //XMStoreFloat3(&emitter.OrientFront, XMVector3Normalize(XMLoadFloat3(&listener.Position) - XMLoadFloat3(&emitter.Position)));
        music_inst->Apply3D(listener, emitter, false);
    }
};
