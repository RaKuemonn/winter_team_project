
#include "framework.h"

#include "scene_game.h"
#include "scene_title.h"
#include "scene_select.h"
#include "scene_loading.h"
#include "scene_title_game.h"

float fps{ 0 };

bool Framework::initialize()
{
	HRESULT hr{ S_OK }; //�����ɏ����l�̐���(�֐��̖߂�l)������


	//�t���[���쐬
	GenerateFrame(hr);


	//�V�[���}�l�[�W���[����
	scene_manager = std::make_unique<Scene_Manager>();
	scene_manager->initialize(device.Get(), immediate_context.Get(), render_target_view.Get(), depth_stencil_view.Get(), hr);
	
	scene_manager->state_manager()->setSS(SS::POINT);
	scene_manager->state_manager()->setSS(SS::LINEAR);
	scene_manager->state_manager()->setSS(SS::ANISOTROPIC);


	//scene_manager->change_scene(new Scene_Game);

	//scene_manager->change_scene(new Scene_Loading(new Scene_Game));
	scene_manager->change_scene(new Scene_Loading(new Scene_Title_Game));
	//scene_manager->change_scene(new Scene_Title_Game);

	


	/*************�r���[�|�[�g�̐ݒ�**************/

	D3D11_VIEWPORT viewport{};

	viewport.TopLeftX = 0;									//��ʍ����x���W
	viewport.TopLeftY = 0;									//��ʍ����y���W
	viewport.Width = static_cast<float>(SCREEN_WIDTH);		//��ʂ̉��T�C�Y
	viewport.Height = static_cast<float>(SCREEN_HEIGHT);	//��ʂ̏c�T�C�Y
	viewport.MinDepth = 0.0f;								//�[�x�l�̍ŏ��l
	viewport.MaxDepth = 1.0f;								//�[�x�l�̍ő�l

	immediate_context->RSSetViewports(1, &viewport);		//�r���[�|�[�g��ݒ�

	/*************�����܂�**************/

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


	//UINT sync_interval{ 0 }; //1�ɂ����60FPS�̌Œ�t���[�����[�g�œ��삷��

	//�����_�����O���ꂽ�摜��`�悷��
	swap_chain->Present(
		sync_interval,		//�t���[���̕\���𐂒��u�����N�Ɠ���������@
		0					//�X���b�v�`�F�[���\���I�v�V����
	);
}


bool Framework::uninitialize()
{
	scene_manager->uninitialize();

	return true;
}


void Framework::GenerateFrame(HRESULT hr)
{
	/*************�f�o�C�X�E�f�o�C�X�R���e�L�X�g�E�X���b�v�`�F�[���̍쐬**************/

	UINT creat_device_flags{ 0 }; //�����^�C�����C���̃t���O
#ifdef _DEBUG
	creat_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG


	D3D_FEATURE_LEVEL feature_levels{ D3D_FEATURE_LEVEL_11_0 }; //�t���[�`���[���x��



	DXGI_SWAP_CHAIN_DESC swap_chain_desc{}; //�X���b�v�`�F�[�����쐬����̂ɕK�v�ȍ\����

	//�X���b�v�`�F�[���̃o�b�t�@��
	swap_chain_desc.BufferCount = 1;		//(�o�b�t�@��2)

	//�o�b�N�o�b�t�@�̐ݒ�(�E�B���h�E�̃T�C�Y��t�H�[�}�b�g)
	swap_chain_desc.BufferDesc.Width = SCREEN_WIDTH;
	swap_chain_desc.BufferDesc.Height = SCREEN_HEIGHT;
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;

	//�o�b�t�@�̎g�p���@�̎w��
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	//�o�͐�E�B���h�E�n���h��
	swap_chain_desc.OutputWindow = hwnd;

	//�}���`�T���v�����O�̐ݒ�(����͎g�p���Ȃ��ݒ�)
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;

	//�E�B���h�E���[�h or �t���X�N���[�����[�h
	swap_chain_desc.Windowed = !FULLSCREEN;


	//�f�o�C�X�A�f�o�C�X�R���e�L�X�g�A�X���b�v�`�F�[�����쐬(����͑S�ē����ɍ쐬����֐����g�p)
	hr = D3D11CreateDeviceAndSwapChain(
		NULL,										//�g�p����O���{�A�_�v�^�̎w��(null�̏ꍇ�K��̃A�_�v�^���g�p)
		D3D_DRIVER_TYPE_HARDWARE,					//�`����s���h���C�o�̎w��(��{�̓n�[�h�E�F�A���w��)
		NULL,										//�h���C�o�̎w����\�t�g�E�F�A�ɂ����ꍇ�ɂ̂ݎw�肪�K�v(����ȊO��null)
		creat_device_flags,							//�����^�C�����C���̃t���O�w��(�f�o�b�O�o�́A�V���O���X���b�h�Ȃǎw��ł���)
		&feature_levels,							//FeatureLevel�̎w��(�z��Ŏ��������o�[�W�����𕡐��w��ł���)
		1,											//D3D_FEATURE_LEVEL���w�肵���ꍇ�̔z��̗v�f��
		D3D11_SDK_VERSION,							//DirectSDK�̃o�[�W����(11�Ȃ�D3D11_SDK_VERSION���w��)
		&swap_chain_desc,							//�ݒ���s����DXGI_SWAP_CHAIN_DESC�̃A�h���X
		swap_chain.GetAddressOf(),					//����������������IDXGISwapChain���󂯎�邽�߂�IDXGISwapChain�̃|�C���^�̃A�h���X
		device.GetAddressOf(),						//����������������ID3D11Device���󂯎�邽�߂�ID3D11Device�̃|�C���^�̃A�h���X
		NULL,										//�ŏI�I�Ɍ��肵��D3D_FEATURE_LEVEL���擾���邽�߂�D3D_FEATURE_LEVEL�̃A�h���X
		immediate_context.GetAddressOf()			//�쐬���ꂽID3D11DeviceContext���󂯎�邽�߂�ID3D11DeviceContext�̃|�C���^�̃A�h���X
	);

	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	/*************�����܂�**************/


	/*************�����_�[�^�[�Q�b�g�r���[�̍쐬**************/

	ID3D11Texture2D* back_buffer{};
	//�X���b�v�`�F�[���̃o�b�t�@�̈���擾����
	hr = swap_chain->GetBuffer(
		0,											//�o�b�t�@�̃C���f�b�N�X(��{��0)
		__uuidof(ID3D11Texture2D),					//�擾����o�b�t�@�̃C���^�[�t�F�[�XID
		reinterpret_cast<LPVOID*>(&back_buffer)		//�o�b�t�@�̎擾��(void�|�C���^�̃A�h���X�Ŏ擾����)
	);

	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	//�����_�[�^�[�Q�b�g�r���[���쐬
	hr = device->CreateRenderTargetView(
		back_buffer,							//�쐬����o�b�t�@�̃��\�[�X
		NULL,									//�쐬����View�̐ݒ���e�f�[�^�̎w��(null�Ńf�t�H���g�ݒ�ɂȂ�)
		render_target_view.GetAddressOf()		//�쐬���ꂽRenderTargetView���i�[���邽�߂̃|�C���^�̃A�h���X
	);

	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	back_buffer->Release(); //�o�b�t�@�����

	/*************�����܂�**************/


	/*************�[�x�X�e���V���r���[�̍쐬**************/

	ID3D11Texture2D* depth_stencil_buffer{}; //�[�x�X�e���V���̃o�b�t�@
	D3D11_TEXTURE2D_DESC texture2d_desc{}; //�e�N�X�`���̐ݒ�

	texture2d_desc.Width = SCREEN_WIDTH;					//�o�b�t�@�̉���
	texture2d_desc.Height = SCREEN_HEIGHT;					//�o�b�t�@�̏c��
	texture2d_desc.MipLevels = 1;							//�~�b�v�}�b�v�̃��x���w��
	texture2d_desc.ArraySize = 1;							//�e�N�X�`���z��̃T�C�Y�w��
	texture2d_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	//�e�N�X�`���̃t�H�[�}�b�g(Depth24bit�AStencil8bit)
	texture2d_desc.SampleDesc.Count = 1;					//�}���`�T���v�����O�ݒ�(�g�p���Ȃ�)
	texture2d_desc.SampleDesc.Quality = 0;					//�}���`�T���v�����O�ݒ�(�g�p���Ȃ�)
	texture2d_desc.Usage = D3D11_USAGE_DEFAULT;				//�e�N�X�`���̎g�p���@
	texture2d_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;	//Bind�ݒ�(D3D11_BIND_DEPTH_STENCIL���w�肷��)
	texture2d_desc.CPUAccessFlags = 0;						//���\�[�X�ւ�CPU�̃A�N�Z�X�����ݒ�(�f�t�H���g��0�Ŗ��Ȃ�)
	texture2d_desc.MiscFlags = 0;							//���\�[�X�I�v�V�����̃t���O(����̃o�b�t�@�ł͕K�v�Ȃ��̂�0�Ŗ��Ȃ�)

	//�w�肳�ꂽ�������ɂ����e�N�X�`�����쐬
	hr = device->CreateTexture2D(
		&texture2d_desc,			//�쐬����e�N�X�`���̏ڍ׏��
		NULL,						//�e�N�X�`���̏��ɕt�^����f�[�^�w�肷��(�ǂݍ��񂾉摜�f�[�^��ݒ肵���肷��)
		&depth_stencil_buffer		//�쐬���ꂽTexture���i�[���邽�߂̃|�C���^�̃A�h���X
	);

	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	//�[�x�X�e���V���r���[�̐ݒ�
	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc{};

	depth_stencil_view_desc.Format = texture2d_desc.Format;						//View�̃t�H�[�}�b�g
	depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;		//�e�N�X�`���̎��(�����Őݒ肵���l�ɂ����D3D11_TEX2D_DSV�����܂�)
	depth_stencil_view_desc.Texture2D.MipSlice = 0;								//�g�p����Texture�f�[�^���


	//�[�x�X�e���V���r���[�̍쐬
	hr = device->CreateDepthStencilView(
		depth_stencil_buffer,							//View�쐬�Ɏg�p���郊�\�[�X
		&depth_stencil_view_desc,						//DSV�Ɋւ���ݒ���s�����f�[�^
		depth_stencil_view.GetAddressOf()				//�쐬���ꂽDSV���i�[���邽�߂̃|�C���^�̃A�h���X
	);

	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	depth_stencil_buffer->Release(); //�o�b�t�@�����

	/*************�����܂�**************/
}
