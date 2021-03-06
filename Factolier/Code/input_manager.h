#pragma once

//#define NOMINMAX	// 学習用にコメントアウト
#include <Windows.h>
#include "Keyboard.h"
#include "GamePad.h"



// キーラベル
constexpr int PAD_UP        = (1 << 0);
constexpr int PAD_DOWN      = (1 << 1);
constexpr int PAD_LEFT      = (1 << 2);
constexpr int PAD_RIGHT     = (1 << 3);
constexpr int PAD_START     = (1 << 4);
constexpr int PAD_TRG1      = (1 << 5);
constexpr int PAD_TRG2      = (1 << 6);
constexpr int PAD_TRG3      = (1 << 7);
constexpr int PAD_SELECT    = (1 << 8);
constexpr int PAD_TRG4      = (1 << 9);
constexpr int PAD_L1        = (1 << 10);
constexpr int PAD_R1        = (1 << 11);
constexpr int PAD_L2        = (1 << 12);
constexpr int PAD_R2        = (1 << 13);
constexpr int PAD_L3        = (1 << 14);
constexpr int PAD_R3        = (1 << 15);
constexpr int KEY_SPACE     = (1 << 16);
constexpr int MOUSE_LEFT    = (1 << 17);
constexpr int KEY_ESC       = (1 << 18);
constexpr int KEY_BACK      = (1 << 19);
constexpr int KEY_L_SHIFT   = (1 << 20);

// パッドの最大数
constexpr int GAMEPAD_NUM   = 4;

// ゲームパッド
enum GamePad
{
    UP      = 0,// dpad
    DOWN,
    RIGHT,
    LEFT,
    A       = 4, // buttons
    B,
    X,
    Y,
    LSTICK,
    RSTICK,
    LSHOULDER,
    RSHOULDER,
    BACK,
    START,
};


// キー割り当てデータ用構造体
struct PadAssign
{
    int         bit;				// PAD_STATE用ビットデータ
    int         code;				// 対応するキー（ボタン）コード
};

//==============================================================================
//
//      入力情報クラス
//
//==============================================================================
class PadState
{
public:

    int         state;              // キー入力情報
    int         trigger;            // トリガー入力
    int         triggerUp;          // 離した瞬間
    int         repeat;             // リピート入力
    int         old[8];
    float       leftX, leftY;       // 左スティック
    float       rightX, rightY;     // 右スティック
    float       left, right;        // 左右トリガー
    PadAssign* keyAssign;          // キーボード割り当てデータ
    PadAssign* joyAssign;          // ジョイスティック割り当てデータ
};

//==============================================================================
//
//      InputManagerクラス
//
//==============================================================================
class Input_Manager
{
protected:
    DirectX::Keyboard* keyboard = nullptr;   // https://github.com/Microsoft/DirectXTK/wiki/Keyboard
    DirectX::GamePad* gamePad = nullptr;    // https://github.com/Microsoft/DirectXTK/wiki/GamePad

    PadState     pad[GAMEPAD_NUM] = {};  // ゲームパッド
    POINT        mousePos = {}; 	            // マウス座標

    // キー・ボタン割り当て
    void setKeyAssign(int no, PadAssign* data); // キー割り当ての設定
    void setJoyAssign(int no, PadAssign* data); // ジョイスティック割り当ての設定

public:
    Input_Manager();
    virtual ~Input_Manager();


    // パッド
    PadState* getPadAddress() { return pad; }	// ゲームパッドの取得

    int TRG(int player_index) { return getPadAddress()[player_index].trigger; }
    int TRG_RELEASE(int player_index) { return getPadAddress()[player_index].triggerUp; }
    int STATE(int player_index) { return getPadAddress()[player_index].state; }
    int REPEAT(int player_index) { return getPadAddress()[player_index].repeat; }

    // マウス関連
    int getCursorPosX();                        // x座標取得
    int getCursorPosY();                        // y座標取得
};

class Input_Manager_Interface final : public Input_Manager
{
public:
    // 更新
    void update(HWND);                          // 入力情報の更新
};