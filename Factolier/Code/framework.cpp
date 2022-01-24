
#include "framework.h"

#include "scene_game.h"
#include "scene_title.h"
#include "scene_select.h"
#include "scene_loading.h"
#include "scene_title_game.h"

float fps{ 0 };
bool quit = false;

bool Framework::initialize()
{
	HRESULT hr{ S_OK }; //


	//
	GenerateFrame(hr);


	//
	scene_manager = std::make_unique<Scene_Manager>();
	scene_manager->initialize(device.Get(), immediate_context.Get(), render_target_view.Get(), depth_stencil_view.Get(), hr);
	
	scene_manager->state_manager()->setSS(SS::POINT);
	scene_manager->state_manager()->setSS(SS::LINEAR);
	scene_manager->state_manager()->setSS(SS::ANISOTROPIC);


	//scene_manager->change_scene(new Scene_Game);

	//scene_manager->change_scene(new Scene_Loading(new Scene_Game));
	scene_manager->change_scene(new Scene_Loading(new Scene_Title_Game));
	//scene_manager->change_scene(new Scene_Title_Game);

	


	/************* **************/

	D3D11_VIEWPORT viewport{};

	viewport.TopLeftX = 0;									//
	viewport.TopLeftY = 0;									//
	viewport.Width = static_cast<float>(SCREEN_WIDTH);		//
	viewport.Height = static_cast<float>(SCREEN_HEIGHT);	//
	viewport.MinDepth = 0.0f;								//
	viewport.MaxDepth = 1.0f;								//

	immediate_context->RSSetViewports(1, &viewport);		//

	/************* **************/

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

	if (quit)
	{
		DestroyWindow(hwnd);
	}

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

	//
	swap_chain->Present(
		sync_interval,		//
		0					//
	);
}


bool Framework::uninitialize()
{
	scene_manager->uninitialize();

	return true;
}


void Framework::GenerateFrame(HRESULT hr)
{
	/************* **************/

	UINT creat_device_flags{ 0 }; //
#ifdef _DEBUG
	creat_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG


	D3D_FEATURE_LEVEL feature_levels{ D3D_FEATURE_LEVEL_11_0 }; //



	DXGI_SWAP_CHAIN_DESC swap_chain_desc{}; //

	//
	swap_chain_desc.BufferCount = 1;		//

	//
	swap_chain_desc.BufferDesc.Width = SCREEN_WIDTH;
	swap_chain_desc.BufferDesc.Height = SCREEN_HEIGHT;
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;

	//
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	//
	swap_chain_desc.OutputWindow = hwnd;

	//
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;

	//
	swap_chain_desc.Windowed = !FULLSCREEN;


	//
	hr = D3D11CreateDeviceAndSwapChain(
		NULL,										//
		D3D_DRIVER_TYPE_HARDWARE,					//
		NULL,										//
		creat_device_flags,							//
		&feature_levels,							//
		1,											//
		D3D11_SDK_VERSION,							//
		&swap_chain_desc,							//
		swap_chain.GetAddressOf(),					//
		device.GetAddressOf(),						//
		NULL,										//
		immediate_context.GetAddressOf()			//
	);

	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	/************* **************/


	/************* **************/

	ID3D11Texture2D* back_buffer{};
	//
	hr = swap_chain->GetBuffer(
		0,											//
		__uuidof(ID3D11Texture2D),					//
		reinterpret_cast<LPVOID*>(&back_buffer)		//
	);

	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	//
	hr = device->CreateRenderTargetView(
		back_buffer,							//
		NULL,									//
		render_target_view.GetAddressOf()		//
	);

	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	back_buffer->Release(); //

	/************* **************/


	/************* **************/

	ID3D11Texture2D* depth_stencil_buffer{}; //
	D3D11_TEXTURE2D_DESC texture2d_desc{}; //

	texture2d_desc.Width = SCREEN_WIDTH;					//
	texture2d_desc.Height = SCREEN_HEIGHT;					//
	texture2d_desc.MipLevels = 1;							//
	texture2d_desc.ArraySize = 1;							//
	texture2d_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	//
	texture2d_desc.SampleDesc.Count = 1;					//
	texture2d_desc.SampleDesc.Quality = 0;					//
	texture2d_desc.Usage = D3D11_USAGE_DEFAULT;				//
	texture2d_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;	//
	texture2d_desc.CPUAccessFlags = 0;						//
	texture2d_desc.MiscFlags = 0;							//

	//
	hr = device->CreateTexture2D(
		&texture2d_desc,			//
		NULL,						//
		&depth_stencil_buffer		//
	);

	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	//
	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc{};

	depth_stencil_view_desc.Format = texture2d_desc.Format;						//
	depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;		//
	depth_stencil_view_desc.Texture2D.MipSlice = 0;								//

	//
	hr = device->CreateDepthStencilView(
		depth_stencil_buffer,							//
		&depth_stencil_view_desc,						//
		depth_stencil_view.GetAddressOf()				//
	);

	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	depth_stencil_buffer->Release(); //

	/************* **************/
}
