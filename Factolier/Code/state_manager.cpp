
#include "misc.h"
#include "state_manager.h"


void State_Manager::generate_state(ID3D11Device* device, HRESULT hr)
{
	/*************サンプラーステートオブジェクトの生成**************/

	D3D11_SAMPLER_DESC sampler_desc;

	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;		//テクスチャをサンプリングするときに使用するフィルタリング方法
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;			//0から1の範囲外のauテクスチャ座標の解決方法
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;			//0から1の範囲外のavテクスチャ座標の解決方法
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;			//0から1の範囲外のawテクスチャ座標の解決方法
	sampler_desc.MipLODBias = 0;								//計算されたミップマップレベルからのオフセット
	sampler_desc.MaxAnisotropy = 16;							//フィルタでANISOTROPICまたはANISOTROPICが指定されている場合に使用されるクランプ値
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;		//サンプリングされたデータを既存のサンプリングされたデータと比較する関数
	sampler_desc.BorderColor[0] = 0;							//ADDRESSにBORDERが指定されている場合に使用する境界線の色
	sampler_desc.BorderColor[1] = 0;							//ADDRESSにBORDERが指定されている場合に使用する境界線の色
	sampler_desc.BorderColor[2] = 0;							//ADDRESSにBORDERが指定されている場合に使用する境界線の色
	sampler_desc.BorderColor[3] = 0;							//ADDRESSにBORDERが指定されている場合に使用する境界線の色
	sampler_desc.MinLOD = 0;									//アクセスをクランプするミップマップ範囲の下限
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;					//アクセスをクランプするミップマップ範囲の上限


	hr = device->CreateSamplerState(
		&sampler_desc,							//サンプラーデスクのアドレス
		sampler_states[0].GetAddressOf()		//作成されたサンプラーステートオブジェクトのアドレス
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	hr = device->CreateSamplerState(
		&sampler_desc,							//サンプラーデスクのアドレス
		sampler_states[1].GetAddressOf()		//作成されたサンプラーステートオブジェクトのアドレス
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
	hr = device->CreateSamplerState(
		&sampler_desc,							//サンプラーデスクのアドレス
		sampler_states[2].GetAddressOf()		//作成されたサンプラーステートオブジェクトのアドレス
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	/*************ここまで**************/


	/*************深度ステンシルステートオブジェクトの生成**************/

	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc{};

	depth_stencil_desc.DepthEnable = TRUE;								//深度テストの有無
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;		//深度ライトの有無
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;			//深度データを既存の深度データと比較する関数


	//深度テスト：オン　深度ライト：オン
	hr = device->CreateDepthStencilState(
		&depth_stencil_desc,						//深度ステンシルデスクのアドレス
		depth_stencil_state[0].GetAddressOf()		//作成された深度ステンシルステートオブジェクトのアドレス
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//深度テスト：オン　深度ライト：オフ
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	hr = device->CreateDepthStencilState(
		&depth_stencil_desc,						//深度ステンシルデスクのアドレス
		depth_stencil_state[1].GetAddressOf()		//作成された深度ステンシルステートオブジェクトのアドレス
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//深度テスト：オフ　深度ライト：オン
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_stencil_desc.DepthEnable = FALSE;
	hr = device->CreateDepthStencilState(
		&depth_stencil_desc,						//深度ステンシルデスクのアドレス
		depth_stencil_state[2].GetAddressOf()		//作成された深度ステンシルステートオブジェクトのアドレス
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//深度テスト：オフ　深度ライト：オフ
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	hr = device->CreateDepthStencilState(
		&depth_stencil_desc,						//深度ステンシルデスクのアドレス
		depth_stencil_state[3].GetAddressOf()		//作成された深度ステンシルステートオブジェクトのアドレス
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	/*************ここまで**************/


	/*************ブレンディングステートオブジェクトの生成**************/

	D3D11_BLEND_DESC blend_desc{};


	//ブレンドモード : NONE
	blend_desc.AlphaToCoverageEnable = FALSE;												//ピクセルをレンダーターゲットに設定するとき、アルファからカバレッジをマルチサンプリング手法として使用するかどうか
	blend_desc.IndependentBlendEnable = FALSE;												//同時レンダーターゲットで独立したブレンディングを有効にするかどうか
	blend_desc.RenderTarget[0].BlendEnable = FALSE;											//ブレンディングの有無
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;									//ピクセルシェーダーが出力するRGB値に対して実行する操作を指定
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;								//レンダーターゲットの現在のRGB値に対して実行する操作を指定
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;								//結合する方法を定義しSrcBlendとDestBlend動作を制御
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;								//ピクセルシェーダーが出力するアルファ値に対して実行する操作を指定
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;							//レンダーターゲットの現在のアルファ値に対して実行する操作を指定
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;							//どのように組み合わせるかを定義しSrcBlendAlphaとDestBlendAlpha動作を制御
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;		//書き込みマスク

	hr = device->CreateBlendState(
		&blend_desc,						//デスクのアドレス
		blend_states[0].GetAddressOf()		//作成されたブレンディングステートオブジェクトのアドレス
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//ブレンドモード : ALPHA
	blend_desc.AlphaToCoverageEnable = FALSE;												//ピクセルをレンダーターゲットに設定するとき、アルファからカバレッジをマルチサンプリング手法として使用するかどうか
	blend_desc.IndependentBlendEnable = FALSE;												//同時レンダーターゲットで独立したブレンディングを有効にするかどうか
	blend_desc.RenderTarget[0].BlendEnable = TRUE;											//ブレンディングの有無
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;							//ピクセルシェーダーが出力するRGB値に対して実行する操作を指定
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;						//レンダーターゲットの現在のRGB値に対して実行する操作を指定
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;								//結合する方法を定義しSrcBlendとDestBlend動作を制御
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;								//ピクセルシェーダーが出力するアルファ値に対して実行する操作を指定
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;					//レンダーターゲットの現在のアルファ値に対して実行する操作を指定
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;							//どのように組み合わせるかを定義しSrcBlendAlphaとDestBlendAlpha動作を制御
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;		//書き込みマスク

	hr = device->CreateBlendState(
		&blend_desc,						//デスクのアドレス
		blend_states[1].GetAddressOf()		//作成されたブレンディングステートオブジェクトのアドレス
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//ブレンドモード : ADD
	blend_desc.AlphaToCoverageEnable = FALSE;												//ピクセルをレンダーターゲットに設定するとき、アルファからカバレッジをマルチサンプリング手法として使用するかどうか
	blend_desc.IndependentBlendEnable = FALSE;												//同時レンダーターゲットで独立したブレンディングを有効にするかどうか
	blend_desc.RenderTarget[0].BlendEnable = TRUE;											//ブレンディングの有無
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;							//ピクセルシェーダーが出力するRGB値に対して実行する操作を指定
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;									//レンダーターゲットの現在のRGB値に対して実行する操作を指定
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;								//結合する方法を定義しSrcBlendとDestBlend動作を制御
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;							//ピクセルシェーダーが出力するアルファ値に対して実行する操作を指定
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;							//レンダーターゲットの現在のアルファ値に対して実行する操作を指定
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;							//どのように組み合わせるかを定義しSrcBlendAlphaとDestBlendAlpha動作を制御
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;		//書き込みマスク

	hr = device->CreateBlendState(
		&blend_desc,						//デスクのアドレス
		blend_states[2].GetAddressOf()		//作成されたブレンディングステートオブジェクトのアドレス
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//ブレンドモード : SUBTRACT
	blend_desc.AlphaToCoverageEnable = FALSE;												//ピクセルをレンダーターゲットに設定するとき、アルファからカバレッジをマルチサンプリング手法として使用するかどうか
	blend_desc.IndependentBlendEnable = FALSE;												//同時レンダーターゲットで独立したブレンディングを有効にするかどうか
	blend_desc.RenderTarget[0].BlendEnable = TRUE;											//ブレンディングの有無
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;							//ピクセルシェーダーが出力するRGB値に対して実行する操作を指定
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;									//レンダーターゲットの現在のRGB値に対して実行する操作を指定
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_SUBTRACT;							//結合する方法を定義しSrcBlendとDestBlend動作を制御
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;							//ピクセルシェーダーが出力するアルファ値に対して実行する操作を指定
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;							//レンダーターゲットの現在のアルファ値に対して実行する操作を指定
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;							//どのように組み合わせるかを定義しSrcBlendAlphaとDestBlendAlpha動作を制御
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;		//書き込みマスク

	hr = device->CreateBlendState(
		&blend_desc,						//デスクのアドレス
		blend_states[3].GetAddressOf()		//作成されたブレンディングステートオブジェクトのアドレス
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//ブレンドモード : REPLACE
	blend_desc.AlphaToCoverageEnable = FALSE;												//ピクセルをレンダーターゲットに設定するとき、アルファからカバレッジをマルチサンプリング手法として使用するかどうか
	blend_desc.IndependentBlendEnable = FALSE;												//同時レンダーターゲットで独立したブレンディングを有効にするかどうか
	blend_desc.RenderTarget[0].BlendEnable = TRUE;											//ブレンディングの有無
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;							//ピクセルシェーダーが出力するRGB値に対して実行する操作を指定
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;								//レンダーターゲットの現在のRGB値に対して実行する操作を指定
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;								//結合する方法を定義しSrcBlendとDestBlend動作を制御
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;								//ピクセルシェーダーが出力するアルファ値に対して実行する操作を指定
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;							//レンダーターゲットの現在のアルファ値に対して実行する操作を指定
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;							//どのように組み合わせるかを定義しSrcBlendAlphaとDestBlendAlpha動作を制御
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;		//書き込みマスク

	hr = device->CreateBlendState(
		&blend_desc,						//デスクのアドレス
		blend_states[4].GetAddressOf()		//作成されたブレンディングステートオブジェクトのアドレス
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//ブレンドモード : MULTIPLY
	blend_desc.AlphaToCoverageEnable = FALSE;												//ピクセルをレンダーターゲットに設定するとき、アルファからカバレッジをマルチサンプリング手法として使用するかどうか
	blend_desc.IndependentBlendEnable = FALSE;												//同時レンダーターゲットで独立したブレンディングを有効にするかどうか
	blend_desc.RenderTarget[0].BlendEnable = TRUE;											//ブレンディングの有無
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;									//ピクセルシェーダーが出力するRGB値に対して実行する操作を指定
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_COLOR;							//レンダーターゲットの現在のRGB値に対して実行する操作を指定
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;								//結合する方法を定義しSrcBlendとDestBlend動作を制御
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_DEST_ALPHA;						//ピクセルシェーダーが出力するアルファ値に対して実行する操作を指定
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;							//レンダーターゲットの現在のアルファ値に対して実行する操作を指定
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;							//どのように組み合わせるかを定義しSrcBlendAlphaとDestBlendAlpha動作を制御
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;		//書き込みマスク

	hr = device->CreateBlendState(
		&blend_desc,						//デスクのアドレス
		blend_states[5].GetAddressOf()		//作成されたブレンディングステートオブジェクトのアドレス
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//ブレンドモード : LIGHTEN
	blend_desc.AlphaToCoverageEnable = FALSE;												//ピクセルをレンダーターゲットに設定するとき、アルファからカバレッジをマルチサンプリング手法として使用するかどうか
	blend_desc.IndependentBlendEnable = FALSE;												//同時レンダーターゲットで独立したブレンディングを有効にするかどうか
	blend_desc.RenderTarget[0].BlendEnable = TRUE;											//ブレンディングの有無
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;									//ピクセルシェーダーが出力するRGB値に対して実行する操作を指定
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;									//レンダーターゲットの現在のRGB値に対して実行する操作を指定
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MAX;								//結合する方法を定義しSrcBlendとDestBlend動作を制御
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;								//ピクセルシェーダーが出力するアルファ値に対して実行する操作を指定
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;							//レンダーターゲットの現在のアルファ値に対して実行する操作を指定
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;							//どのように組み合わせるかを定義しSrcBlendAlphaとDestBlendAlpha動作を制御
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;		//書き込みマスク

	hr = device->CreateBlendState(
		&blend_desc,						//デスクのアドレス
		blend_states[6].GetAddressOf()		//作成されたブレンディングステートオブジェクトのアドレス
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//ブレンドモード : DARKEN
	blend_desc.AlphaToCoverageEnable = FALSE;												//ピクセルをレンダーターゲットに設定するとき、アルファからカバレッジをマルチサンプリング手法として使用するかどうか
	blend_desc.IndependentBlendEnable = FALSE;												//同時レンダーターゲットで独立したブレンディングを有効にするかどうか
	blend_desc.RenderTarget[0].BlendEnable = TRUE;											//ブレンディングの有無
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;									//ピクセルシェーダーが出力するRGB値に対して実行する操作を指定
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;									//レンダーターゲットの現在のRGB値に対して実行する操作を指定
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MIN;								//結合する方法を定義しSrcBlendとDestBlend動作を制御
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;								//ピクセルシェーダーが出力するアルファ値に対して実行する操作を指定
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;							//レンダーターゲットの現在のアルファ値に対して実行する操作を指定
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MIN;							//どのように組み合わせるかを定義しSrcBlendAlphaとDestBlendAlpha動作を制御
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;		//書き込みマスク

	hr = device->CreateBlendState(
		&blend_desc,						//デスクのアドレス
		blend_states[7].GetAddressOf()		//作成されたブレンディングステートオブジェクトのアドレス
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//ブレンドモード : SCREEN
	blend_desc.AlphaToCoverageEnable = FALSE;												//ピクセルをレンダーターゲットに設定するとき、アルファからカバレッジをマルチサンプリング手法として使用するかどうか
	blend_desc.IndependentBlendEnable = FALSE;												//同時レンダーターゲットで独立したブレンディングを有効にするかどうか
	blend_desc.RenderTarget[0].BlendEnable = TRUE;											//ブレンディングの有無
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;							//ピクセルシェーダーが出力するRGB値に対して実行する操作を指定
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;						//レンダーターゲットの現在のRGB値に対して実行する操作を指定
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;								//結合する方法を定義しSrcBlendとDestBlend動作を制御
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;								//ピクセルシェーダーが出力するアルファ値に対して実行する操作を指定
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;					//レンダーターゲットの現在のアルファ値に対して実行する操作を指定
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;							//どのように組み合わせるかを定義しSrcBlendAlphaとDestBlendAlpha動作を制御
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;		//書き込みマスク

	hr = device->CreateBlendState(
		&blend_desc,						//デスクのアドレス
		blend_states[8].GetAddressOf()		//作成されたブレンディングステートオブジェクトのアドレス
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//ブレンドモード : COVERAGE
	blend_desc.AlphaToCoverageEnable = TRUE;												//ピクセルをレンダーターゲットに設定するとき、アルファからカバレッジをマルチサンプリング手法として使用するかどうか
	blend_desc.IndependentBlendEnable = FALSE;												//同時レンダーターゲットで独立したブレンディングを有効にするかどうか
	blend_desc.RenderTarget[0].BlendEnable = TRUE;											//ブレンディングの有無
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;							//ピクセルシェーダーが出力するRGB値に対して実行する操作を指定
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;						//レンダーターゲットの現在のRGB値に対して実行する操作を指定
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;								//結合する方法を定義しSrcBlendとDestBlend動作を制御
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;								//ピクセルシェーダーが出力するアルファ値に対して実行する操作を指定
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;					//レンダーターゲットの現在のアルファ値に対して実行する操作を指定
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;							//どのように組み合わせるかを定義しSrcBlendAlphaとDestBlendAlpha動作を制御
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;		//書き込みマスク

	hr = device->CreateBlendState(
		&blend_desc,						//デスクのアドレス
		blend_states[9].GetAddressOf()		//作成されたブレンディングステートオブジェクトのアドレス
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	/*************ここまで**************/


	/*************ラスタライザーステートオブジェクトの生成**************/

	D3D11_RASTERIZER_DESC rasterizer_desc{};
	rasterizer_desc.FillMode = D3D11_FILL_SOLID;		//レンダリング時に使用する塗りつぶしモード
	rasterizer_desc.CullMode = D3D11_CULL_BACK;			//指定された方向を向いている三角形が描画されていないことを示す
	rasterizer_desc.FrontCounterClockwise = TRUE;		//三角形が前面か背面かを決定
	rasterizer_desc.DepthBias = 0;						//特定のピクセルに追加された深度値
	rasterizer_desc.DepthBiasClamp = 0;					//ピクセルの最大深度
	rasterizer_desc.SlopeScaledDepthBias = 0;			//特定のピクセルの傾斜上のスカラー
	rasterizer_desc.DepthClipEnable = TRUE;				//距離に基づいてクリッピングを有効にする
	rasterizer_desc.ScissorEnable = FALSE;				//長方形のカリングを有効にする
	rasterizer_desc.MultisampleEnable = FALSE;			//マルチサンプルアンチエイリアシング
	rasterizer_desc.AntialiasedLineEnable = FALSE;		//回線アンチエイリアス

	//塗りつぶし/裏面カリング
	hr = device->CreateRasterizerState(&rasterizer_desc, rasterizer_state[0].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//塗りつぶし/カリングなし
	rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	rasterizer_desc.CullMode = D3D11_CULL_NONE;
	rasterizer_desc.AntialiasedLineEnable = FALSE;

	hr = device->CreateRasterizerState(&rasterizer_desc, rasterizer_state[1].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//ワイヤーフレーム/裏面カリング
	rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizer_desc.CullMode = D3D11_CULL_BACK;
	rasterizer_desc.AntialiasedLineEnable = TRUE;

	hr = device->CreateRasterizerState(&rasterizer_desc, rasterizer_state[2].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//ワイヤーフレーム/カリングなし
	rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizer_desc.CullMode = D3D11_CULL_NONE;
	rasterizer_desc.AntialiasedLineEnable = TRUE;

	hr = device->CreateRasterizerState(&rasterizer_desc, rasterizer_state[3].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	/*************ここまで**************/
}