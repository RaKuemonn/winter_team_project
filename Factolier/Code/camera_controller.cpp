#include "camera_controller.h"
#include "camera.h"
#include "utility.h"

//更新処理
void Camera_Controller::update(ID3D11DeviceContext* dc, Input_Manager* input_manager, float elapsed_time)
{
    if (input_manager->TRG_RELEASE(0) & PAD_SELECT)
    {
        SetCursorPos(static_cast<int>(SCREEN_WIDTH / 2), static_cast<int>(SCREEN_HEIGHT / 2));
        ShowCursor(false);
    }

    if (input_manager->TRG(0) & PAD_SELECT)
    {
        ShowCursor(true);
    }


    if (input_manager->STATE(0) & PAD_SELECT)
    {
        
    }

    else
    {
        //マウス取得
        DirectX::XMFLOAT2 mouse = { CAST_F(input_manager->getCursorPosX()), CAST_F(input_manager->getCursorPosY()) };

        //ビューポート
        D3D11_VIEWPORT viewport;
        UINT numViewports = 1;
        dc->RSGetViewports(&numViewports, &viewport);

        //変換行列
        DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&Camera::Instance().get_view());
        DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&Camera::Instance().get_projection());
        DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();


        //画面中央の座標
        DirectX::XMFLOAT3 DefPos = { viewport.Width / 2, viewport.Height / 2, viewport.MinDepth };

        //二つのワールド座標に変換
        DirectX::XMVECTOR NearPos = DirectX::XMVector3Unproject(
            DirectX::XMLoadFloat3(&DefPos),		        //スクリーン座標(near)
            viewport.TopLeftX,							//ビューポート左上X位置
            viewport.TopLeftY,							//ビューポート左上Y位置
            viewport.Width,								//ビューポート幅
            viewport.Height,							//ビューポート高さ
            viewport.MinDepth,							//深度値の範囲を表す最小値(0.0でよい)
            viewport.MaxDepth,							//深度値の範囲を表す最大値(1.0でよい)
            Projection,									//プロジェクション行列
            View,										//ビュー行列
            World										//ワールド行列(単位行列でよい)
        );

        DefPos.z = viewport.MaxDepth;
        DirectX::XMVECTOR FarPos = DirectX::XMVector3Unproject(
            DirectX::XMLoadFloat3(&DefPos),		        //スクリーン座標(far)
            viewport.TopLeftX,							//ビューポート左上X位置
            viewport.TopLeftY,							//ビューポート左上Y位置
            viewport.Width,								//ビューポート幅
            viewport.Height,							//ビューポート高さ
            viewport.MinDepth,							//深度値の範囲を表す最小値(0.0でよい)
            viewport.MaxDepth,							//深度値の範囲を表す最大値(1.0でよい)
            Projection,									//プロジェクション行列
            View,										//ビュー行列
            World										//ワールド行列(単位行列でよい)
        );

        //デフォルトのレイを作成
        DirectX::XMVECTOR DefRayVec = DirectX::XMVectorSubtract(FarPos, NearPos);
        DefRayVec = DirectX::XMVector3Normalize(DefRayVec);

        DirectX::XMFLOAT3 ScreenPosition;

        if (mouse.x != static_cast<int>(viewport.Width / 2))
        {
            //マウスカーソル座標を取得
            ScreenPosition.x = mouse.x;
            ScreenPosition.y = viewport.Height / 2;
            ScreenPosition.z = viewport.MaxDepth;

            //ワールド座標に変換
            DirectX::XMVECTOR FarPosX = DirectX::XMVector3Unproject(
                DirectX::XMLoadFloat3(&ScreenPosition),		//スクリーン座標(far)
                viewport.TopLeftX,							//ビューポート左上X位置
                viewport.TopLeftY,							//ビューポート左上Y位置
                viewport.Width,								//ビューポート幅
                viewport.Height,							//ビューポート高さ
                viewport.MinDepth,							//深度値の範囲を表す最小値(0.0でよい)
                viewport.MaxDepth,							//深度値の範囲を表す最大値(1.0でよい)
                Projection,									//プロジェクション行列
                View,										//ビュー行列
                World										//ワールド行列(単位行列でよい)
            );

            //マウスのレイを作成
            DirectX::XMVECTOR MouseRayVecX = DirectX::XMVectorSubtract(FarPosX, NearPos);
            MouseRayVecX = DirectX::XMVector3Normalize(MouseRayVecX);

            //回転した角度を求める
            float angleX;
            DirectX::XMStoreFloat(&angleX, DirectX::XMVector3Dot(MouseRayVecX, DefRayVec));
            if (mouse.x > (viewport.Width / 2)) angleX = acosf(angleX);
            else angleX = -acosf(angleX);

            //現在の角度に加算する
            angle.y += angleX * sens;
        }

        if (mouse.y != static_cast<int>(viewport.Height / 2))
        {
            //マウスカーソル座標を取得
            ScreenPosition.x = viewport.Width / 2;
            ScreenPosition.y = mouse.y;
            ScreenPosition.z = viewport.MaxDepth;

            //ワールド座標に変換
            DirectX::XMVECTOR FarPosY = DirectX::XMVector3Unproject(
                DirectX::XMLoadFloat3(&ScreenPosition),		//スクリーン座標(far)
                viewport.TopLeftX,							//ビューポート左上X位置
                viewport.TopLeftY,							//ビューポート左上Y位置
                viewport.Width,								//ビューポート幅
                viewport.Height,							//ビューポート高さ
                viewport.MinDepth,							//深度値の範囲を表す最小値(0.0でよい)
                viewport.MaxDepth,							//深度値の範囲を表す最大値(1.0でよい)
                Projection,									//プロジェクション行列
                View,										//ビュー行列
                World										//ワールド行列(単位行列でよい)
            );

            //マウスのレイを作成
            DirectX::XMVECTOR MouseRayVecY = DirectX::XMVectorSubtract(FarPosY, NearPos);
            MouseRayVecY = DirectX::XMVector3Normalize(MouseRayVecY);

            //回転した角度を求める
            float angleY;
            DirectX::XMStoreFloat(&angleY, DirectX::XMVector3Dot(MouseRayVecY, DefRayVec));
            if (mouse.y > (viewport.Height / 2)) angleY = acosf(angleY);
            else angleY = -acosf(angleY);

            //現在の角度に加算する
            angle.x += angleY * sens;
        }

        SetCursorPos(static_cast<int>(viewport.Width / 2), static_cast<int>(viewport.Height / 2));
    }



    if(GetAsyncKeyState('I'))//if (input_manager->STATE(0) & PAD_UP)
    {
        angle.x += 1.0f * elapsed_time;
    }

    if (GetAsyncKeyState('K'))//if (input_manager->STATE(0) & PAD_DOWN)
    {
        angle.x -= 1.0f * elapsed_time;
    }

    if (GetAsyncKeyState('J'))//if (input_manager->STATE(0) & PAD_LEFT)
    {
        angle.y += 1.0f * elapsed_time;
    }

    if (GetAsyncKeyState('L'))//if (input_manager->STATE(0) & PAD_RIGHT)
    {
        angle.y -= 1.0f * elapsed_time;
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

    constexpr float range = 15.0f;

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