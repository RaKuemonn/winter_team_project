#include "camera_controller.h"
#include "camera.h"
//#include "Input/Input.h"

//�X�V����
void Camera_Controller::update(float elapsedTime, Input_Manager* input_manager)
{
#if 0
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisRX();
    float ay = gamePad.GetAxisRY();
    //�J�����̉�]���x
    float speed = roll_speed * elapsedTime;

    //�X�e�B�b�N�̓��͒l�ɍ��킹��X����Y������]
    angle.y += ax * speed;
    angle.x += ay * speed;
#endif

    if (input_manager->STATE(0) & PAD_UP)
    {
        angle.x += 1.0f * elapsedTime;
    }

    if (input_manager->STATE(0) & PAD_DOWN)
    {
        angle.x -= 1.0f * elapsedTime;
    }

    if (input_manager->STATE(0) & PAD_LEFT)
    {
        angle.y += 1.0f * elapsedTime;
    }

    if (input_manager->STATE(0) & PAD_RIGHT)
    {
        angle.y -= 1.0f * elapsedTime;
    }


    //X���̃J������]�𐧌�
    if (angle.x > max_angle)
    {
        angle.x = max_angle;
    }
    
    if (angle.x < min_angle)
    {
        angle.x = min_angle;
    }

    //Y���̉�]�l��-3.14~3.14�Ɏ��܂�悤�ɂ���
    if (angle.y < -DirectX::XM_PI)
    {
        angle.y += DirectX::XM_2PI;
    }

    if (angle.y > DirectX::XM_PI)
    {
        angle.y -= DirectX::XM_2PI;
    }

    //�J������]�l����]�s��ɕϊ�
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    //��]�s���ۑ�
    DirectX::XMFLOAT4X4 romat;
    DirectX::XMStoreFloat4x4(&romat, Transform);
    Camera::Instance().set_rotation_matrix(romat);

    //��]�s�񂩂�O�����x�N�g�������o��
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    //�����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
    DirectX::XMFLOAT3 eye;
    eye.x = target.x - front.x * range;
    eye.y = target.y - front.y * range;
    eye.z = target.z - front.z * range;

    //�J�����̎��_�ƒ����_��ݒ�
    Camera::Instance().set_lookat(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}