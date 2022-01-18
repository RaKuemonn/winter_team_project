#include "camera_controller.h"
#include "camera.h"
//#include "Input/Input.h"

//更新処理
void Camera_Controller::update(float elapsedTime, Input_Manager* input_manager)
{
#if 0
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisRX();
    float ay = gamePad.GetAxisRY();
    //カメラの回転速度
    float speed = roll_speed * elapsedTime;

    //スティックの入力値に合わせてX軸とY軸を回転
    angle.y += ax * speed;
    angle.x += ay * speed;
#endif



    if(GetAsyncKeyState('I'))//if (input_manager->STATE(0) & PAD_UP)
    {
        angle.x += 1.0f * elapsedTime;
    }

    if (GetAsyncKeyState('K'))//if (input_manager->STATE(0) & PAD_DOWN)
    {
        angle.x -= 1.0f * elapsedTime;
    }

    if (GetAsyncKeyState('J'))//if (input_manager->STATE(0) & PAD_LEFT)
    {
        angle.y += 1.0f * elapsedTime;
    }

    if (GetAsyncKeyState('L'))//if (input_manager->STATE(0) & PAD_RIGHT)
    {
        angle.y -= 1.0f * elapsedTime;
    }


    constexpr float	max_angle = DirectX::XMConvertToRadians(80);
    constexpr float	min_angle = DirectX::XMConvertToRadians(-80);


    //X軸のカメラ回転を制限
    if (angle.x > max_angle)
    {
        angle.x = max_angle;
    }
    
    if (angle.x < min_angle)
    {
        angle.x = min_angle;
    }

    //Y軸の回転値を-3.14~3.14に収まるようにする
    if (angle.y < -DirectX::XM_PI)
    {
        angle.y += DirectX::XM_2PI;
    }

    if (angle.y > DirectX::XM_PI)
    {
        angle.y -= DirectX::XM_2PI;
    }

    //カメラ回転値を回転行列に変換
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    //回転行列から前方向ベクトルを取り出す
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    //注視点から後ろベクトル方向に一定距離離れたカメラ視点を求める
    DirectX::XMFLOAT3 eye;

    if (ptr_target == nullptr)
    {
        eye.x = target.x - front.x * range;
        eye.y = target.y - front.y * range;
        eye.z = target.z - front.z * range;
        //カメラの視点と注視点を設定
        Camera::Instance().set_lookat(eye, target, DirectX::XMFLOAT3(0, 1, 0));
    }

    else
    {
        eye.x = ptr_target->x - front.x * range;
        eye.y = ptr_target->y - front.y * range;
        eye.z = ptr_target->z - front.z * range;
        //カメラの視点と注視点を設定
        Camera::Instance().set_lookat(eye, *ptr_target, DirectX::XMFLOAT3(0, 1, 0));
    }

}