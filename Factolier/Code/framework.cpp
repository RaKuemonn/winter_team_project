
#include "framework.h"

#include "scene_game.h"
#include "scene_title.h"
#include "scene_select.h"
#include "scene_loading.h"
#include "scene_title_game.h"

float fps{ 0 };

bool Framework::initialize()
{
	HRESULT hr{ S_OK }; //ここに初期値の成否(関数の戻り値)が入る


	//フレーム作成
	GenerateFrame(hr);


	//シーンマネージャー生成
	scene_manager = std::make_unique<Scene_Manager>();
	scene_manager->initialize(device.Get(), immediate_context.Get(), render_target_view.Get(), depth_stencil_view.Get(), hr);

	scene_manager->change_scene(new Scene_Title_Game);
	//scene_manager->change_scene(new Scene_Loading(new Scene_Game));
	//scene_manager->change_scene(new Scene_Select);
	//scene_manager->change_scene(new Scene_Loading(new Scene_Title));
	//scene_manager->change_scene(new Scene_Game);
	


	/*************ビューポートの設定**************/

	D3D11_VIEWPORT viewport{};

	viewport.TopLeftX = 0;									//画面左上のx座標
	viewport.TopLeftY = 0;									//画面左上のy座標
	viewport.Width = static_cast<float>(SCREEN_WIDTH);		//画面の横サイズ
	viewport.Height = static_cast<float>(SCREEN_HEIGHT);	//画面の縦サイズ
	viewport.MinDepth = 0.0f;								//深度値の最小値
	viewport.MaxDepth = 1.0f;								//深度値の最大値

	immediate_context->RSSetViewports(1, &viewport);		//ビューポートを設定

	/*************ここまで**************/

	ShowCursor(false);

	return true;
}


void Framework::update(float elapsed_time/*Elapsed seconds from last frame*/)
{
#ifdef USE_IMGUI
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
#endif

	scene_manager->update(elapsed_time, hwnd);

#ifdef USE_IMGUI
	
#endif
}


void Framework::render(float elapsed_time/*Elapsed seconds from last frame*/)
{
	std::lock_guard<std::mutex> lock(scene_manager->mutex());

	//FLOAT color[]{ 1.0f, 1.0f, 1.0f, 1.0f };
	//
	//immediate_context->ClearRenderTargetView(render_target_view.Get(), color);
	//immediate_context->ClearDepthStencilView(depth_stencil_view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//immediate_context->OMSetRenderTargets(1, render_target_view.GetAddressOf(), depth_stencil_view.Get());
	
	//immediate_context->OMSetRenderTargets(1, render_target_view.GetAddressOf(), nullptr);



	constexpr DirectX::XMFLOAT4X4 coordinate_system_transforms[]{
		{ -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },	// 0:RHS Y-UP
		{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },		// 1:LHS Y-UP
		{ -1, 0, 0, 0, 0, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },	// 2:RHS Z-UP
		{ 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },		// 3:LHS Z-UP
	};


	ImGui::Begin("framework");

	ImGui::InputFloat("fps", &fps);

	ImGui::End();


	scene_manager->render(elapsed_time);


#ifdef USE_IMGUI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif


	//UINT sync_interval{ 0 }; //1にすると60FPSの固定フレームレートで動作する

	//レンダリングされた画像を描画する
	swap_chain->Present(
		sync_interval,		//フレームの表示を垂直ブランクと同期する方法
		0					//スワップチェーン表示オプション
	);
}


bool Framework::uninitialize()
{
	scene_manager->uninitialize();

	return true;
}


void Framework::GenerateFrame(HRESULT hr)
{
	/*************デバイス・デバイスコンテキスト・スワップチェーンの作成**************/

	UINT creat_device_flags{ 0 }; //ランタイムレイヤのフラグ
#ifdef _DEBUG
	creat_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG


	D3D_FEATURE_LEVEL feature_levels{ D3D_FEATURE_LEVEL_11_0 }; //フューチャーレベル



	DXGI_SWAP_CHAIN_DESC swap_chain_desc{}; //スワップチェーンを作成するのに必要な構造体

	//スワップチェーンのバッファ数
	swap_chain_desc.BufferCount = 1;		//(バッファ数2)

	//バックバッファの設定(ウィンドウのサイズやフォーマット)
	swap_chain_desc.BufferDesc.Width = SCREEN_WIDTH;
	swap_chain_desc.BufferDesc.Height = SCREEN_HEIGHT;
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;

	//バッファの使用方法の指定
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	//出力先ウィンドウハンドル
	swap_chain_desc.OutputWindow = hwnd;

	//マルチサンプリングの設定(今回は使用しない設定)
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;

	//ウィンドウモード or フルスクリーンモード
	swap_chain_desc.Windowed = !FULLSCREEN;


	//デバイス、デバイスコンテキスト、スワップチェーンを作成(今回は全て同時に作成する関数を使用)
	hr = D3D11CreateDeviceAndSwapChain(
		NULL,										//使用するグラボアダプタの指定(nullの場合規定のアダプタを使用)
		D3D_DRIVER_TYPE_HARDWARE,					//描画を行うドライバの指定(基本はハードウェアを指定)
		NULL,										//ドライバの指定をソフトウェアにした場合にのみ指定が必要(それ以外はnull)
		creat_device_flags,							//ランタイムレイヤのフラグ指定(デバッグ出力、シングルスレッドなど指定できる)
		&feature_levels,							//FeatureLevelの指定(配列で試したいバージョンを複数指定できる)
		1,											//D3D_FEATURE_LEVELを指定した場合の配列の要素数
		D3D11_SDK_VERSION,							//DirectSDKのバージョン(11ならD3D11_SDK_VERSIONを指定)
		&swap_chain_desc,							//設定を行ったDXGI_SWAP_CHAIN_DESCのアドレス
		swap_chain.GetAddressOf(),					//初期化が完了したIDXGISwapChainを受け取るためのIDXGISwapChainのポインタのアドレス
		device.GetAddressOf(),						//初期化が完了したID3D11Deviceを受け取るためのID3D11Deviceのポインタのアドレス
		NULL,										//最終的に決定したD3D_FEATURE_LEVELを取得するためのD3D_FEATURE_LEVELのアドレス
		immediate_context.GetAddressOf()			//作成されたID3D11DeviceContextを受け取るためのID3D11DeviceContextのポインタのアドレス
	);

	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	/*************ここまで**************/


	/*************レンダーターゲットビューの作成**************/

	ID3D11Texture2D* back_buffer{};
	//スワップチェーンのバッファの一つを取得する
	hr = swap_chain->GetBuffer(
		0,											//バッファのインデックス(基本は0)
		__uuidof(ID3D11Texture2D),					//取得するバッファのインターフェースID
		reinterpret_cast<LPVOID*>(&back_buffer)		//バッファの取得先(voidポインタのアドレスで取得する)
	);

	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	//レンダーターゲットビューを作成
	hr = device->CreateRenderTargetView(
		back_buffer,							//作成するバッファのリソース
		NULL,									//作成するViewの設定内容データの指定(nullでデフォルト設定になる)
		render_target_view.GetAddressOf()		//作成されたRenderTargetViewを格納するためのポインタのアドレス
	);

	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	back_buffer->Release(); //バッファを解放

	/*************ここまで**************/


	/*************深度ステンシルビューの作成**************/

	ID3D11Texture2D* depth_stencil_buffer{}; //深度ステンシルのバッファ
	D3D11_TEXTURE2D_DESC texture2d_desc{}; //テクスチャの設定

	texture2d_desc.Width = SCREEN_WIDTH;					//バッファの横幅
	texture2d_desc.Height = SCREEN_HEIGHT;					//バッファの縦幅
	texture2d_desc.MipLevels = 1;							//ミップマップのレベル指定
	texture2d_desc.ArraySize = 1;							//テクスチャ配列のサイズ指定
	texture2d_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	//テクスチャのフォーマット(Depth24bit、Stencil8bit)
	texture2d_desc.SampleDesc.Count = 1;					//マルチサンプリング設定(使用しない)
	texture2d_desc.SampleDesc.Quality = 0;					//マルチサンプリング設定(使用しない)
	texture2d_desc.Usage = D3D11_USAGE_DEFAULT;				//テクスチャの使用方法
	texture2d_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;	//Bind設定(D3D11_BIND_DEPTH_STENCILを指定する)
	texture2d_desc.CPUAccessFlags = 0;						//リソースへのCPUのアクセス権限設定(デフォルトは0で問題ない)
	texture2d_desc.MiscFlags = 0;							//リソースオプションのフラグ(今回のバッファでは必要ないので0で問題ない)

	//指定された情報を元にしたテクスチャを作成
	hr = device->CreateTexture2D(
		&texture2d_desc,			//作成するテクスチャの詳細情報
		NULL,						//テクスチャの情報に付与するデータ指定する(読み込んだ画像データを設定したりする)
		&depth_stencil_buffer		//作成されたTextureを格納するためのポインタのアドレス
	);

	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	//深度ステンシルビューの設定
	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc{};

	depth_stencil_view_desc.Format = texture2d_desc.Format;						//Viewのフォーマット
	depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;		//テクスチャの種類(ここで設定した値によってD3D11_TEX2D_DSVが決まる)
	depth_stencil_view_desc.Texture2D.MipSlice = 0;								//使用するTextureデータ情報


	//深度ステンシルビューの作成
	hr = device->CreateDepthStencilView(
		depth_stencil_buffer,							//View作成に使用するリソース
		&depth_stencil_view_desc,						//DSVに関する設定を行ったデータ
		depth_stencil_view.GetAddressOf()				//作成されたDSVを格納するためのポインタのアドレス
	);

	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	depth_stencil_buffer->Release(); //バッファを解放

	/*************ここまで**************/
}
