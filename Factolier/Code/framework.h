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

#ifdef USE_IMGUI
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern ImWchar glyphRangesJapanese[];
#endif

CONST LONG SCREEN_WIDTH{ 1280 };
CONST LONG SCREEN_HEIGHT{ 720 };
CONST BOOL FULLSCREEN{ FALSE };
CONST LPWSTR APPLICATION_NAME{ L"X3DGP" };

extern float fps;

class Framework
{
public:
	CONST HWND hwnd;

	//COM�I�u�W�F�N�g��錾//

	//DX11�̃f�o�C�X
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	//DX11�̕`�施�߂𑗂邽�߂̂���
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context;
	//�n�[�h�E�F�A�̏�񂪋l�܂��Ă������
	Microsoft::WRL::ComPtr<IDXGISwapChain> swap_chain;
	//�f�B�X�v���C�̃o�b�N�o�b�t�@�̃e�N�X�`����`���Ƃ��Ďw��ł���悤�ɂ�������
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_view;
	//�[��(Z��)�̏��������Ă������(���s����\�����邽�߂̉��ߕ`����)
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depth_stencil_view;


	/*
	�f�o�C�X�R���e�L�X�g�ɂ�Immediate��Deferred�̓��ނ�����܂��B

	Immediate�͐��������R�}���h�𑦎����s���܂��B
	D3D11CreateDeviceAndSwapChain�ō쐬�����ꍇ�AImmediate�ō쐬����܂��B

	Deferred�͐��������`��R�}���h���o�b�t�@�ɂ��߂Ă����܂��B
	�����āA���s���߂��󂯂��炽�܂��Ă����R�}���h�����s���܂��B
	Deferred�̓}���`�X���b�h�ɂ��`�揈���ɗL���Ƃ���Ă��܂��B
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
				update(tictoc.time_interval());
				render(tictoc.time_interval());
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
	float elapsed_time{ 0.0f };
	void calculate_frame_stats()
	{
		if (++frames, (tictoc.time_stamp() - elapsed_time) >= 1.0f)
		{
			fps = static_cast<float>(frames);
			std::wostringstream outs;
			outs.precision(6);
			outs << APPLICATION_NAME << L" : FPS : " << fps << L" / " << L"Frame Time : " << 1000.0f / fps << L" (ms)";
			SetWindowTextW(hwnd, outs.str().c_str());

			frames = 0;
			elapsed_time += 1.0f;
		}
	}
};

