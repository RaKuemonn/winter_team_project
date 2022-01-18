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
    //コンストラクタ
    Sound(std::shared_ptr<DirectX::SoundEffect> resource, float volume = (0.5f))
    {
        if (music_inst) music_inst.reset();
        music = resource;
        def_volume = volume;
        cur_volume = volume;
    }


    //デストラクタ
    ~Sound()
    {
        unload();
    }

    
    //更新処理
    void update()
    {
        // ポーズ中や停止中はフェードを行わない
        if (is_in_use() && get_state() != DirectX::SoundState::PLAYING)
            return;

        // フェードイン
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

        // フェードアウト
        if (add < 0)
        {
            cur_volume += add;
            if (cur_volume < fade_volume)
            {
                cur_volume = fade_volume;
                add = 0.0f;
            }
            //set_volume(cur_volume);

            // ボリュームが0以下になったら停止
            if (cur_volume <= 0.0f)
            {
                stop();
            }
        }
    }


    //破棄
    void unload()
    {
        if (music_inst)  music_inst.reset();
        if (music)      music.reset();
    }

    
    //再生
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

    
    //停止
    void stop()
    {
        if (!music_inst) return;
        music_inst->Stop();
    }


    //ポーズ
    void pause()
    {
        if (!music_inst) return;
        music_inst->Pause();
    }

    
    //レジューム（ポーズからの復帰）
    void resume()
    {
        if (!music_inst) return;
        music_inst->Resume();
    }

    
    //ボリュームの設定
    void set_volume(float volume)
    {
        if (!music_inst) return;
        music_inst->SetVolume(volume);
        cur_volume = volume;
    }

    
    //ボリュームの取得
    float get_volume()
    {
        return cur_volume;
    }


    //フェードイン・フェードアウト
    void fade(float fade_sec, float volume)
    {
        fade_volume = volume;
        add = (fade_volume - cur_volume) / (fps * fade_sec);   // 1フレームあたりのボリュームの変位
    }

    
    //状態の取得（isInUseがtrueの場合のみ行うこと）
    DirectX::SoundState get_state()
    {
        return music_inst->GetState();
    }

    
    //ループの取得
    bool is_looped()
    {
        return music_inst->IsLooped();
    }

    
    //パンの設定（左右のバランス）
    void set_pan(float p)
    {
        pan = p;
        music_inst->SetPan(pan);
    }

    
    //パンの取得（左右のバランス）
    float get_pan()
    {
        return pan;
    }

    
    //パンの調整
    float adjust_pan(float p)
    {
        pan += p;
        pan = (std::max)((std::min)(pan, 1.0f), -1.0f);
        music_inst->SetPan(pan);
        return pan;
    }

    
    //ピッチの設定
    void set_pitch(float p)
    {
        pitch = p;
        music_inst->SetPitch(pitch);
    }

    
    //ピッチの取得
    float get_pitch()
    {
        return pitch;
    }

    
    //ピッチの調整
    float adjust_pitch(float p)
    {
        pitch += p;
        pitch = (std::max)((std::min)(pitch, 1.0f), -1.0f);
        music_inst->SetPitch(pitch);
        return pitch;
    }

    
    //フォーマットの取得
    const WAVEFORMATEX* get_format()
    {
        return music->GetFormat();
    }

    
    //音楽が使用可能かどうか
    bool is_in_use()
    {
        if (!music) return false;
        return music->IsInUse();
    }


    //エミッター設定
    void set_emitter(const DirectX::XMFLOAT3 pos)
    {
        emitter.SetPosition(pos);
        //emitter.OrientTop = { 0.0f, 1.0f, 0.0f };
        emitter.ChannelCount = music->GetFormat()->nChannels;
        //emitter.ChannelRadius = 20.0f;
        //emitter.CurveDistanceScaler = 1.4f;
    }


    //３Dエフェクト適用
    void apply_3d(const DirectX::AudioListener& listener)
    {
        using namespace DirectX;
        //XMStoreFloat3(&emitter.OrientFront, XMVector3Normalize(XMLoadFloat3(&listener.Position) - XMLoadFloat3(&emitter.Position)));
        music_inst->Apply3D(listener, emitter, false);
    }
};
