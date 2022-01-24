
#include "camera_controller.h"
#include "camera.h"
#include "utility.h"
#include "easing.h"

//�X�V����
void Camera_Controller::update(ID3D11DeviceContext* dc, Input_Manager* input_manager, Option_Manager* option_manager, float elapsed_time)
{
    sens = option_manager->camera_move * CAMERA_MAX;

    if (input_manager->TRG_RELEASE(0) & PAD_SELECT)
    {
        bool i = SetCursorPos(static_cast<int>(SCREEN_WIDTH / 2), static_cast<int>(SCREEN_HEIGHT / 2));
        ShowCursor(false);
    }
    
    if (input_manager->TRG(0) & PAD_SELECT)
    {
        ShowCursor(true);
    }

    // TODO: debug
#ifdef _DEBUG
    if (input_manager->STATE(0) & PAD_SELECT || input_manager->TRG_RELEASE(0) & PAD_SELECT)
    {
    }

    else
    {
        //�}�E�X�擾
        DirectX::XMFLOAT2 mouse = { CAST_F(input_manager->getCursorPosX()), CAST_F(input_manager->getCursorPosY()) };

        //�r���[�|�[�g
        D3D11_VIEWPORT viewport;
        UINT numViewports = 1;
        dc->RSGetViewports(&numViewports, &viewport);

        //�ϊ��s��
        DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&Camera::Instance().get_view());
        DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&Camera::Instance().get_projection());
        DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();


        //��ʒ����̍��W
        DirectX::XMFLOAT3 DefPos = { viewport.Width / 2, viewport.Height / 2, viewport.MinDepth };

        //��̃��[���h���W�ɕϊ�
        DirectX::XMVECTOR NearPos = DirectX::XMVector3Unproject(
            DirectX::XMLoadFloat3(&DefPos),		        //�X�N���[�����W(near)
            viewport.TopLeftX,							//�r���[�|�[�g����X�ʒu
            viewport.TopLeftY,							//�r���[�|�[�g����Y�ʒu
            viewport.Width,								//�r���[�|�[�g��
            viewport.Height,							//�r���[�|�[�g����
            viewport.MinDepth,							//�[�x�l�͈̔͂�\���ŏ��l(0.0�ł悢)
            viewport.MaxDepth,							//�[�x�l�͈̔͂�\���ő�l(1.0�ł悢)
            Projection,									//�v���W�F�N�V�����s��
            View,										//�r���[�s��
            World										//���[���h�s��(�P�ʍs��ł悢)
        );

        DefPos.z = viewport.MaxDepth;
        DirectX::XMVECTOR FarPos = DirectX::XMVector3Unproject(
            DirectX::XMLoadFloat3(&DefPos),		        //�X�N���[�����W(far)
            viewport.TopLeftX,							//�r���[�|�[�g����X�ʒu
            viewport.TopLeftY,							//�r���[�|�[�g����Y�ʒu
            viewport.Width,								//�r���[�|�[�g��
            viewport.Height,							//�r���[�|�[�g����
            viewport.MinDepth,							//�[�x�l�͈̔͂�\���ŏ��l(0.0�ł悢)
            viewport.MaxDepth,							//�[�x�l�͈̔͂�\���ő�l(1.0�ł悢)
            Projection,									//�v���W�F�N�V�����s��
            View,										//�r���[�s��
            World										//���[���h�s��(�P�ʍs��ł悢)
        );

        //�f�t�H���g�̃��C���쐬
        DirectX::XMVECTOR DefRayVec = DirectX::XMVectorSubtract(FarPos, NearPos);
        DefRayVec = DirectX::XMVector3Normalize(DefRayVec);

        DirectX::XMFLOAT3 ScreenPosition;

        if (mouse.x != static_cast<int>(viewport.Width / 2))
        {
            //�}�E�X�J�[�\�����W���擾
            ScreenPosition.x = mouse.x;
            ScreenPosition.y = viewport.Height / 2;
            ScreenPosition.z = viewport.MaxDepth;

            //���[���h���W�ɕϊ�
            DirectX::XMVECTOR FarPosX = DirectX::XMVector3Unproject(
                DirectX::XMLoadFloat3(&ScreenPosition),		//�X�N���[�����W(far)
                viewport.TopLeftX,							//�r���[�|�[�g����X�ʒu
                viewport.TopLeftY,							//�r���[�|�[�g����Y�ʒu
                viewport.Width,								//�r���[�|�[�g��
                viewport.Height,							//�r���[�|�[�g����
                viewport.MinDepth,							//�[�x�l�͈̔͂�\���ŏ��l(0.0�ł悢)
                viewport.MaxDepth,							//�[�x�l�͈̔͂�\���ő�l(1.0�ł悢)
                Projection,									//�v���W�F�N�V�����s��
                View,										//�r���[�s��
                World										//���[���h�s��(�P�ʍs��ł悢)
            );

            //�}�E�X�̃��C���쐬
            DirectX::XMVECTOR MouseRayVecX = DirectX::XMVectorSubtract(FarPosX, NearPos);
            MouseRayVecX = DirectX::XMVector3Normalize(MouseRayVecX);

            //��]�����p�x�����߂�
            float angleX;
            DirectX::XMStoreFloat(&angleX, DirectX::XMVector3Dot(MouseRayVecX, DefRayVec));
            if (mouse.x > (viewport.Width / 2)) angleX = acosf(angleX);
            else angleX = -acosf(angleX);

            //���݂̊p�x�ɉ��Z����
            angle.y += angleX * sens;
        }

        if (mouse.y != static_cast<int>(viewport.Height / 2))
        {
            //�}�E�X�J�[�\�����W���擾
            ScreenPosition.x = viewport.Width / 2;
            ScreenPosition.y = mouse.y;
            ScreenPosition.z = viewport.MaxDepth;

            //���[���h���W�ɕϊ�
            DirectX::XMVECTOR FarPosY = DirectX::XMVector3Unproject(
                DirectX::XMLoadFloat3(&ScreenPosition),		//�X�N���[�����W(far)
                viewport.TopLeftX,							//�r���[�|�[�g����X�ʒu
                viewport.TopLeftY,							//�r���[�|�[�g����Y�ʒu
                viewport.Width,								//�r���[�|�[�g��
                viewport.Height,							//�r���[�|�[�g����
                viewport.MinDepth,							//�[�x�l�͈̔͂�\���ŏ��l(0.0�ł悢)
                viewport.MaxDepth,							//�[�x�l�͈̔͂�\���ő�l(1.0�ł悢)
                Projection,									//�v���W�F�N�V�����s��
                View,										//�r���[�s��
                World										//���[���h�s��(�P�ʍs��ł悢)
            );

            //�}�E�X�̃��C���쐬
            DirectX::XMVECTOR MouseRayVecY = DirectX::XMVectorSubtract(FarPosY, NearPos);
            MouseRayVecY = DirectX::XMVector3Normalize(MouseRayVecY);

            //��]�����p�x�����߂�
            float angleY;
            DirectX::XMStoreFloat(&angleY, DirectX::XMVector3Dot(MouseRayVecY, DefRayVec));
            if (mouse.y > (viewport.Height / 2)) angleY = acosf(angleY);
            else angleY = -acosf(angleY);

            //���݂̊p�x�ɉ��Z����
            angle.x += angleY * sens;
        }

        SetCursorPos(static_cast<int>(viewport.Width / 2), static_cast<int>(viewport.Height / 2));
    }
#endif



    if (GetAsyncKeyState('I'))//if (input_manager->STATE(0) & PAD_UP)
    {
        angle.x += 1.0f * elapsed_time * sens;
    }

    if (GetAsyncKeyState('K'))//if (input_manager->STATE(0) & PAD_DOWN)
    {
        angle.x -= 1.0f * elapsed_time * sens;
    }

    if (GetAsyncKeyState('J'))//if (input_manager->STATE(0) & PAD_LEFT)
    {
        angle.y += 1.0f * elapsed_time * sens;
    }

    if (GetAsyncKeyState('L'))//if (input_manager->STATE(0) & PAD_RIGHT)
    {
        angle.y -= 1.0f * elapsed_time * sens;
    }




    constexpr float	max_angle = DirectX::XMConvertToRadians(80);
    constexpr float	min_angle = DirectX::XMConvertToRadians(-80);


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

    //��]�s�񂩂�O�����x�N�g�������o��
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    
    if (input_manager->TRG(0) & MOUSE_LEFT && !clear_flag)
    {
        //clear_flag = true;
    }

    /////////////////// 演出終了フレーム以外毎フレームfalse
    is_performance_end = false;
    ///////////////////

    constexpr float range = 15.0f;

    if (ptr_target == nullptr)
    {
        eye.x = target.x - front.x * range;
        eye.y = target.y - front.y * range;
        eye.z = target.z - front.z * range;

        Camera::Instance().set_lookat(eye, target, DirectX::XMFLOAT3(0, 1, 0));
    }

    else
    {
        if (ptr_target->y > -10.0f && !clear_flag)
        {
            eye.x = ptr_target->x - front.x * range;
            eye.y = ptr_target->y - front.y * range;
            eye.z = ptr_target->z - front.z * range;
        }

        else if (clear_flag)
        {
            clear_timer += elapsed_time;

            if (clear_timer <= 1.0f)
            {
                eye.x = ptr_target->x + 2.0f;
                eye.y = easing::in_out_circ(clear_timer, 1.0f, ptr_target->y + 5.0f, ptr_target->y);
                eye.z = ptr_target->z + 2.0f;
            }

            else if (clear_timer > 1.5f && clear_timer < 2.0f)
            {
                eye.x = ptr_target->x;
                eye.y = ptr_target->y + 1.0f;
                eye.z = easing::in_out_quad(clear_timer - 1.5f, 0.5f, ptr_target->z + 10.0f, ptr_target->z + 5.0f);
            }

            else if (clear_timer > 3.0f)
            {
                clear_timer = 0.0f;
                clear_flag = false;
                is_performance_end = true;
            }
        }

        Camera::Instance().set_lookat(eye, *ptr_target, DirectX::XMFLOAT3(0, 1, 0));
    }

}