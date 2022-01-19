#pragma once

#include <windows.h>
#include <tchar.h>
#include <sstream>

#include "misc.h"
#include "high_resolution_timer.h"


#include <d3d11.h>
#include <wrl.h>
#include <memory>
#include "scene_manager.h"
#include "Keyboard.h"
#include "utility.h"

#ifdef USE_IMGUI
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern ImWchar glyphRangesJapanese[];
#endif

static constexpr int sync_interval = 0;
extern float fps;

class Framework
{
public:
	CONST HWND hwnd;

	//COMオブジェクトを宣言//

	//DX11のデバイス
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	//DX11の描画命令を送るためのもの
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context;
	//ハードウェアの情報が詰まっているもの
	Microsoft::WRL::ComPtr<IDXGISwapChain> swap_chain;
	//ディスプレイのバックバッファのテクスチャを描画先として指定できるようにしたもの
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_view;
	//深さ(Z軸)の情報を持っているもの(奥行きを表現するための遠近描画情報)
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depth_stencil_view;


	/*
	デバイスコンテキストにはImmediateとDeferredの二種類があります。

	Immediateは生成したコマンドを即時実行します。
	D3D11CreateDeviceAndSwapChainで作成した場合、Immediateで作成されます。

	Deferredは生成した描画コマンドをバッファにためておきます。
	そして、実行命令を受けたらたまっていたコマンドを実行します。
	Deferredはマルチスレッドによる描画処理に有効とされています。
	*/


	std::unique_ptr<Scene_Manager> scene_manager = nullptr;


	Framework(HWND hwnd) : hwnd(hwnd) {}
	~Framework() {}

	Framework(const Framework&) = delete;
	Framework& operator=(const Framework&) = delete;
	Framework(Framework&&) noexcept = delete;
	Framework& operator=(Framework&&) noexcept = delete;

	int run()
	{
		MSG msg{};

		if (!initialize())
		{
			return 0;
		}

#ifdef USE_IMGUI
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\consola.ttf", 14.0f, nullptr, glyphRangesJapanese);

		// リリース時imgui.iniを出力させない
#ifdef NDEBUG
		ImGui::GetIO().IniFilename = 0;
#endif

		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplDX11_Init(device.Get(), immediate_context.Get());
		ImGui::StyleColorsDark();
#endif

		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				tictoc.tick();
				calculate_frame_stats();


				float elapsed_time = sync_interval == 0
					? tictoc.time_interval()
					: sync_interval / 60
					;

				update(elapsed_time);
				render(elapsed_time);
			}
		}

#ifdef USE_IMGUI
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
#endif

#if 1
		BOOL fullscreen = 0;
		swap_chain->GetFullscreenState(&fullscreen, 0);
		if (fullscreen)
		{
			swap_chain->SetFullscreenState(FALSE, 0);
		}
#endif

		return uninitialize() ? static_cast<int>(msg.wParam) : 0;
	}

	LRESULT CALLBACK handle_message(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
#ifdef USE_IMGUI
		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) { return true; }
#endif
		switch (msg)
		{
		case WM_PAINT:
		{
			PAINTSTRUCT ps{};
			BeginPaint(hwnd, &ps);

			EndPaint(hwnd, &ps);
		}
		break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_CREATE:
			break;

		case WM_ACTIVATEAPP:
			DirectX::Keyboard::ProcessMessage(msg, wparam, lparam);
			break;

		case WM_KEYDOWN:
			if (wparam == VK_ESCAPE)
			{
				PostMessage(hwnd, WM_CLOSE, 0, 0);
				break;
			}

		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
		case WM_MOUSEFIRST:
			DirectX::Keyboard::ProcessMessage(msg, wparam, lparam);
			break;
			
		case WM_ENTERSIZEMOVE:
			tictoc.stop();
			break;
		case WM_EXITSIZEMOVE:
			tictoc.start();
			break;
		default:
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}
		return 0;
	}

private:
	bool initialize();
	void update(float elapsed_time/*Elapsed seconds from last frame*/);
	void render(float elapsed_time/*Elapsed seconds from last frame*/);
	bool uninitialize();

	void GenerateFrame(HRESULT hr);

private:
	high_resolution_timer tictoc;
	uint32_t frames{ 0 };
    float time_tlapsed = 0.0f;

	void calculate_frame_stats()
	{
		++frames;
		if ((tictoc.time_stamp() - time_tlapsed) >= 1.0f)
		{
			fps = static_cast<float>(frames);
			std::wostringstream outs;
			outs.precision(6);
			outs << APPLICATION_NAME << L" : FPS : " << fps << L" / " << L"Frame Time : " << 1000.0f / fps << L" (ms)";
			SetWindowTextW(hwnd, outs.str().c_str());

			frames = 0;
			time_tlapsed += 1.0f;
		}
	}
};

