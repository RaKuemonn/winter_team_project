
#include "effect_manager.h"


void Effect_Manager::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    //Effekseer�����_������
    effekseer_renderer = EffekseerRendererDX11::Renderer::Create(device, context, 2048);

    //Effekseer�}�l�[�W���[����
    effekseer_manager = Effekseer::Manager::Create(2048);

    //Effekseer�����_���̊e��ݒ�
    effekseer_manager->SetSpriteRenderer(effekseer_renderer->CreateSpriteRenderer());
    effekseer_manager->SetRibbonRenderer(effekseer_renderer->CreateRibbonRenderer());
    effekseer_manager->SetRingRenderer(effekseer_renderer->CreateRingRenderer());
    effekseer_manager->SetTrackRenderer(effekseer_renderer->CreateTrackRenderer());
    effekseer_manager->SetModelRenderer(effekseer_renderer->CreateModelRenderer());
    //Effekseer���ł̃��[�_�[�̐ݒ�(���ʂȃJ�X�^�}�C�Y�����Ȃ��ꍇ�͈ȉ��̐ݒ��OK)
    effekseer_manager->SetTextureLoader(effekseer_renderer->CreateTextureLoader());
    effekseer_manager->SetModelLoader(effekseer_renderer->CreateModelLoader());
    effekseer_manager->SetMaterialLoader(effekseer_renderer->CreateMaterialLoader());

    //Effekseer������n���W�Ōv�Z����
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
    //�G�t�F�N�g�X�V����(�����ɂ̓t���[���̌o�ߎ��Ԃ�n��)
    effekseer_manager->Update(elapsedTime * 60.0f);
}


void Effect_Manager::render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    //�r���[���v���W�F�N�V�����s���Effekseer�����_���ɐݒ�
    effekseer_renderer->SetCameraMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&view));
    effekseer_renderer->SetProjectionMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&projection));

    //Effekseer�`��J�n
    effekseer_renderer->BeginRendering();

    //Effekseer�`����s
    //�}�l�[�W���[�P�ʂŕ`�悷��̂ŕ`�揇�𐧌䂷��ꍇ�̓}�l�[�W���[�𕡐��쐬���A
    //Draw()�֐������s���鏇���Ő���o������
    effekseer_manager->Draw();

    //Effekseer�`��I��
    effekseer_renderer->EndRendering();
}