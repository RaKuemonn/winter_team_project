
#include "effect.h"


Effect::Effect(Effect_Manager* effect_manager, std::mutex& mutex, const char* filename)
{
    //エフェクトを読み込みする前にロックする
    //＊マルチスレッドでEffectを作成するとDeviceContextに同時アクセスして
    //フリーズする可能性があるので排他制御する
    std::lock_guard<std::mutex> lock(mutex);

    //Effekseerのリソースを読み込む
    //EffekseerはUTF-16のファイルパス以外は対応していないため文字コード変換が必要
    char16_t utf16Filename[256];
    Effekseer::ConvertUtf8ToUtf16(utf16Filename, 256, filename);

    //Effekseer::Managerを取得
    Effekseer::Manager* effekseerManager = effect_manager->get_effekseer_manager();

    //Effekseerエフェクトを取得
    effekseer_effect = Effekseer::Effect::Create(effekseerManager, (EFK_CHAR*)utf16Filename);
}


Effect::~Effect()
{
    //破棄処理
    if (effekseer_effect != nullptr)
    {
        effekseer_effect->Release();
        effekseer_effect = nullptr;
    }
}


//再生
Effekseer::Handle Effect::play(const DirectX::XMFLOAT3& position, Effect_Manager* effect_manager, float scale)
{
    Effekseer::Manager* effekseerManager = effect_manager->get_effekseer_manager();

    Effekseer::Handle handle = effekseerManager->Play(effekseer_effect, position.x, position.y, position.z);
    effekseerManager->SetScale(handle, scale, scale, scale);
    return handle;
}


//停止
void Effect::stop(Effekseer::Handle handle, Effect_Manager* effect_manager)
{
    Effekseer::Manager* effekseerManager = effect_manager->get_effekseer_manager();

    effekseerManager->StopEffect(handle);
}


//座標設定
void Effect::set_position(Effekseer::Handle handle, const DirectX::XMFLOAT3& position, Effect_Manager* effect_manager)
{
    Effekseer::Manager* effekseerManager = effect_manager->get_effekseer_manager();

    effekseerManager->SetLocation(handle, position.x, position.y, position.z);
}


//スケール設定
void Effect::set_scale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale, Effect_Manager* effect_manager)
{
    Effekseer::Manager* effekseerManager = effect_manager->get_effekseer_manager();

    effekseerManager->SetScale(handle, scale.x, scale.y, scale.z);
}
