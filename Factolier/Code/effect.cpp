
#include "effect.h"


Effect::Effect(Effect_Manager* effect_manager, std::mutex& mutex, const char* filename)
{
    //�G�t�F�N�g��ǂݍ��݂���O�Ƀ��b�N����
    //���}���`�X���b�h��Effect���쐬�����DeviceContext�ɓ����A�N�Z�X����
    //�t���[�Y����\��������̂Ŕr�����䂷��
    std::lock_guard<std::mutex> lock(mutex);

    //Effekseer�̃��\�[�X��ǂݍ���
    //Effekseer��UTF-16�̃t�@�C���p�X�ȊO�͑Ή����Ă��Ȃ����ߕ����R�[�h�ϊ����K�v
    char16_t utf16Filename[256];
    Effekseer::ConvertUtf8ToUtf16(utf16Filename, 256, filename);

    //Effekseer::Manager���擾
    Effekseer::Manager* effekseerManager = effect_manager->get_effekseer_manager();

    //Effekseer�G�t�F�N�g���擾
    effekseer_effect = Effekseer::Effect::Create(effekseerManager, (EFK_CHAR*)utf16Filename);
}


Effect::~Effect()
{
    //�j������
    if (effekseer_effect != nullptr)
    {
        effekseer_effect->Release();
        effekseer_effect = nullptr;
    }
}


//�Đ�
Effekseer::Handle Effect::play(const DirectX::XMFLOAT3& position, Effect_Manager* effect_manager, float scale)
{
    Effekseer::Manager* effekseerManager = effect_manager->get_effekseer_manager();

    Effekseer::Handle handle = effekseerManager->Play(effekseer_effect, position.x, position.y, position.z);
    effekseerManager->SetScale(handle, scale, scale, scale);
    return handle;
}


//��~
void Effect::stop(Effekseer::Handle handle, Effect_Manager* effect_manager)
{
    Effekseer::Manager* effekseerManager = effect_manager->get_effekseer_manager();

    effekseerManager->StopEffect(handle);
}


//���W�ݒ�
void Effect::set_position(Effekseer::Handle handle, const DirectX::XMFLOAT3& position, Effect_Manager* effect_manager)
{
    Effekseer::Manager* effekseerManager = effect_manager->get_effekseer_manager();

    effekseerManager->SetLocation(handle, position.x, position.y, position.z);
}


//�X�P�[���ݒ�
void Effect::set_scale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale, Effect_Manager* effect_manager)
{
    Effekseer::Manager* effekseerManager = effect_manager->get_effekseer_manager();

    effekseerManager->SetScale(handle, scale.x, scale.y, scale.z);
}
