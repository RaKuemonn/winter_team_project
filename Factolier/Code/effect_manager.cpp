
#include "effect_manager.h"


void Effect_Manager::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    //Effekseerレンダラ生成
    effekseer_renderer = EffekseerRendererDX11::Renderer::Create(device, context, 2048);

    //Effekseerマネージャー生成
    effekseer_manager = Effekseer::Manager::Create(2048);

    //Effekseerレンダラの各種設定
    effekseer_manager->SetSpriteRenderer(effekseer_renderer->CreateSpriteRenderer());
    effekseer_manager->SetRibbonRenderer(effekseer_renderer->CreateRibbonRenderer());
    effekseer_manager->SetRingRenderer(effekseer_renderer->CreateRingRenderer());
    effekseer_manager->SetTrackRenderer(effekseer_renderer->CreateTrackRenderer());
    effekseer_manager->SetModelRenderer(effekseer_renderer->CreateModelRenderer());
    //Effekseer内でのローダーの設定(特別なカスタマイズをしない場合は以下の設定でOK)
    effekseer_manager->SetTextureLoader(effekseer_renderer->CreateTextureLoader());
    effekseer_manager->SetModelLoader(effekseer_renderer->CreateModelLoader());
    effekseer_manager->SetMaterialLoader(effekseer_renderer->CreateMaterialLoader());

    //Effekseerを左手系座標で計算する
    effekseer_manager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
}


void Effect_Manager::finalize()
{
    if (effekseer_manager != nullptr)
    {
        effekseer_manager->Destroy();
        effekseer_manager = nullptr;
    }

    if (effekseer_renderer != nullptr)
    {
        effekseer_renderer->Destroy();
        effekseer_renderer = nullptr;
    }
}


void Effect_Manager::update(float elapsedTime)
{
    //エフェクト更新処理(引数にはフレームの経過時間を渡す)
    effekseer_manager->Update(elapsedTime * 60.0f);
}


void Effect_Manager::render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    //ビュー＆プロジェクション行列をEffekseerレンダラに設定
    effekseer_renderer->SetCameraMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&view));
    effekseer_renderer->SetProjectionMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&projection));

    //Effekseer描画開始
    effekseer_renderer->BeginRendering();

    //Effekseer描画実行
    //マネージャー単位で描画するので描画順を制御する場合はマネージャーを複数個作成し、
    //Draw()関数を実行する順序で制御出来そう
    effekseer_manager->Draw();

    //Effekseer描画終了
    effekseer_renderer->EndRendering();
}