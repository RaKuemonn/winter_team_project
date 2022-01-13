
#include "misc.h"
#include "state_manager.h"


void State_Manager::generate_state(ID3D11Device* device, HRESULT hr)
{
	/*************�T���v���[�X�e�[�g�I�u�W�F�N�g�̐���**************/

	D3D11_SAMPLER_DESC sampler_desc;

	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;		//�e�N�X�`�����T���v�����O����Ƃ��Ɏg�p����t�B���^�����O���@
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;			//0����1�͈̔͊O��au�e�N�X�`�����W�̉������@
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;			//0����1�͈̔͊O��av�e�N�X�`�����W�̉������@
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;			//0����1�͈̔͊O��aw�e�N�X�`�����W�̉������@
	sampler_desc.MipLODBias = 0;								//�v�Z���ꂽ�~�b�v�}�b�v���x������̃I�t�Z�b�g
	sampler_desc.MaxAnisotropy = 16;							//�t�B���^��ANISOTROPIC�܂���ANISOTROPIC���w�肳��Ă���ꍇ�Ɏg�p�����N�����v�l
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;		//�T���v�����O���ꂽ�f�[�^�������̃T���v�����O���ꂽ�f�[�^�Ɣ�r����֐�
	sampler_desc.BorderColor[0] = 0;							//ADDRESS��BORDER���w�肳��Ă���ꍇ�Ɏg�p���鋫�E���̐F
	sampler_desc.BorderColor[1] = 0;							//ADDRESS��BORDER���w�肳��Ă���ꍇ�Ɏg�p���鋫�E���̐F
	sampler_desc.BorderColor[2] = 0;							//ADDRESS��BORDER���w�肳��Ă���ꍇ�Ɏg�p���鋫�E���̐F
	sampler_desc.BorderColor[3] = 0;							//ADDRESS��BORDER���w�肳��Ă���ꍇ�Ɏg�p���鋫�E���̐F
	sampler_desc.MinLOD = 0;									//�A�N�Z�X���N�����v����~�b�v�}�b�v�͈͂̉���
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;					//�A�N�Z�X���N�����v����~�b�v�}�b�v�͈͂̏��


	hr = device->CreateSamplerState(
		&sampler_desc,							//�T���v���[�f�X�N�̃A�h���X
		sampler_states[0].GetAddressOf()		//�쐬���ꂽ�T���v���[�X�e�[�g�I�u�W�F�N�g�̃A�h���X
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	hr = device->CreateSamplerState(
		&sampler_desc,							//�T���v���[�f�X�N�̃A�h���X
		sampler_states[1].GetAddressOf()		//�쐬���ꂽ�T���v���[�X�e�[�g�I�u�W�F�N�g�̃A�h���X
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
	hr = device->CreateSamplerState(
		&sampler_desc,							//�T���v���[�f�X�N�̃A�h���X
		sampler_states[2].GetAddressOf()		//�쐬���ꂽ�T���v���[�X�e�[�g�I�u�W�F�N�g�̃A�h���X
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	/*************�����܂�**************/


	/*************�[�x�X�e���V���X�e�[�g�I�u�W�F�N�g�̐���**************/

	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc{};

	depth_stencil_desc.DepthEnable = TRUE;								//�[�x�e�X�g�̗L��
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;		//�[�x���C�g�̗L��
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;			//�[�x�f�[�^�������̐[�x�f�[�^�Ɣ�r����֐�


	//�[�x�e�X�g�F�I���@�[�x���C�g�F�I��
	hr = device->CreateDepthStencilState(
		&depth_stencil_desc,						//�[�x�X�e���V���f�X�N�̃A�h���X
		depth_stencil_state[0].GetAddressOf()		//�쐬���ꂽ�[�x�X�e���V���X�e�[�g�I�u�W�F�N�g�̃A�h���X
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//�[�x�e�X�g�F�I���@�[�x���C�g�F�I�t
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	hr = device->CreateDepthStencilState(
		&depth_stencil_desc,						//�[�x�X�e���V���f�X�N�̃A�h���X
		depth_stencil_state[1].GetAddressOf()		//�쐬���ꂽ�[�x�X�e���V���X�e�[�g�I�u�W�F�N�g�̃A�h���X
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//�[�x�e�X�g�F�I�t�@�[�x���C�g�F�I��
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_stencil_desc.DepthEnable = FALSE;
	hr = device->CreateDepthStencilState(
		&depth_stencil_desc,						//�[�x�X�e���V���f�X�N�̃A�h���X
		depth_stencil_state[2].GetAddressOf()		//�쐬���ꂽ�[�x�X�e���V���X�e�[�g�I�u�W�F�N�g�̃A�h���X
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//�[�x�e�X�g�F�I�t�@�[�x���C�g�F�I�t
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	hr = device->CreateDepthStencilState(
		&depth_stencil_desc,						//�[�x�X�e���V���f�X�N�̃A�h���X
		depth_stencil_state[3].GetAddressOf()		//�쐬���ꂽ�[�x�X�e���V���X�e�[�g�I�u�W�F�N�g�̃A�h���X
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	/*************�����܂�**************/


	/*************�u�����f�B���O�X�e�[�g�I�u�W�F�N�g�̐���**************/

	D3D11_BLEND_DESC blend_desc{};


	//�u�����h���[�h : NONE
	blend_desc.AlphaToCoverageEnable = FALSE;												//�s�N�Z���������_�[�^�[�Q�b�g�ɐݒ肷��Ƃ��A�A���t�@����J�o���b�W���}���`�T���v�����O��@�Ƃ��Ďg�p���邩�ǂ���
	blend_desc.IndependentBlendEnable = FALSE;												//���������_�[�^�[�Q�b�g�œƗ������u�����f�B���O��L���ɂ��邩�ǂ���
	blend_desc.RenderTarget[0].BlendEnable = FALSE;											//�u�����f�B���O�̗L��
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;									//�s�N�Z���V�F�[�_�[���o�͂���RGB�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;								//�����_�[�^�[�Q�b�g�̌��݂�RGB�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;								//����������@���`��SrcBlend��DestBlend����𐧌�
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;								//�s�N�Z���V�F�[�_�[���o�͂���A���t�@�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;							//�����_�[�^�[�Q�b�g�̌��݂̃A���t�@�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;							//�ǂ̂悤�ɑg�ݍ��킹�邩���`��SrcBlendAlpha��DestBlendAlpha����𐧌�
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;		//�������݃}�X�N

	hr = device->CreateBlendState(
		&blend_desc,						//�f�X�N�̃A�h���X
		blend_states[0].GetAddressOf()		//�쐬���ꂽ�u�����f�B���O�X�e�[�g�I�u�W�F�N�g�̃A�h���X
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//�u�����h���[�h : ALPHA
	blend_desc.AlphaToCoverageEnable = FALSE;												//�s�N�Z���������_�[�^�[�Q�b�g�ɐݒ肷��Ƃ��A�A���t�@����J�o���b�W���}���`�T���v�����O��@�Ƃ��Ďg�p���邩�ǂ���
	blend_desc.IndependentBlendEnable = FALSE;												//���������_�[�^�[�Q�b�g�œƗ������u�����f�B���O��L���ɂ��邩�ǂ���
	blend_desc.RenderTarget[0].BlendEnable = TRUE;											//�u�����f�B���O�̗L��
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;							//�s�N�Z���V�F�[�_�[���o�͂���RGB�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;						//�����_�[�^�[�Q�b�g�̌��݂�RGB�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;								//����������@���`��SrcBlend��DestBlend����𐧌�
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;								//�s�N�Z���V�F�[�_�[���o�͂���A���t�@�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;					//�����_�[�^�[�Q�b�g�̌��݂̃A���t�@�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;							//�ǂ̂悤�ɑg�ݍ��킹�邩���`��SrcBlendAlpha��DestBlendAlpha����𐧌�
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;		//�������݃}�X�N

	hr = device->CreateBlendState(
		&blend_desc,						//�f�X�N�̃A�h���X
		blend_states[1].GetAddressOf()		//�쐬���ꂽ�u�����f�B���O�X�e�[�g�I�u�W�F�N�g�̃A�h���X
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//�u�����h���[�h : ADD
	blend_desc.AlphaToCoverageEnable = FALSE;												//�s�N�Z���������_�[�^�[�Q�b�g�ɐݒ肷��Ƃ��A�A���t�@����J�o���b�W���}���`�T���v�����O��@�Ƃ��Ďg�p���邩�ǂ���
	blend_desc.IndependentBlendEnable = FALSE;												//���������_�[�^�[�Q�b�g�œƗ������u�����f�B���O��L���ɂ��邩�ǂ���
	blend_desc.RenderTarget[0].BlendEnable = TRUE;											//�u�����f�B���O�̗L��
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;							//�s�N�Z���V�F�[�_�[���o�͂���RGB�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;									//�����_�[�^�[�Q�b�g�̌��݂�RGB�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;								//����������@���`��SrcBlend��DestBlend����𐧌�
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;							//�s�N�Z���V�F�[�_�[���o�͂���A���t�@�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;							//�����_�[�^�[�Q�b�g�̌��݂̃A���t�@�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;							//�ǂ̂悤�ɑg�ݍ��킹�邩���`��SrcBlendAlpha��DestBlendAlpha����𐧌�
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;		//�������݃}�X�N

	hr = device->CreateBlendState(
		&blend_desc,						//�f�X�N�̃A�h���X
		blend_states[2].GetAddressOf()		//�쐬���ꂽ�u�����f�B���O�X�e�[�g�I�u�W�F�N�g�̃A�h���X
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//�u�����h���[�h : SUBTRACT
	blend_desc.AlphaToCoverageEnable = FALSE;												//�s�N�Z���������_�[�^�[�Q�b�g�ɐݒ肷��Ƃ��A�A���t�@����J�o���b�W���}���`�T���v�����O��@�Ƃ��Ďg�p���邩�ǂ���
	blend_desc.IndependentBlendEnable = FALSE;												//���������_�[�^�[�Q�b�g�œƗ������u�����f�B���O��L���ɂ��邩�ǂ���
	blend_desc.RenderTarget[0].BlendEnable = TRUE;											//�u�����f�B���O�̗L��
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;							//�s�N�Z���V�F�[�_�[���o�͂���RGB�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;									//�����_�[�^�[�Q�b�g�̌��݂�RGB�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_SUBTRACT;							//����������@���`��SrcBlend��DestBlend����𐧌�
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;							//�s�N�Z���V�F�[�_�[���o�͂���A���t�@�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;							//�����_�[�^�[�Q�b�g�̌��݂̃A���t�@�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;							//�ǂ̂悤�ɑg�ݍ��킹�邩���`��SrcBlendAlpha��DestBlendAlpha����𐧌�
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;		//�������݃}�X�N

	hr = device->CreateBlendState(
		&blend_desc,						//�f�X�N�̃A�h���X
		blend_states[3].GetAddressOf()		//�쐬���ꂽ�u�����f�B���O�X�e�[�g�I�u�W�F�N�g�̃A�h���X
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//�u�����h���[�h : REPLACE
	blend_desc.AlphaToCoverageEnable = FALSE;												//�s�N�Z���������_�[�^�[�Q�b�g�ɐݒ肷��Ƃ��A�A���t�@����J�o���b�W���}���`�T���v�����O��@�Ƃ��Ďg�p���邩�ǂ���
	blend_desc.IndependentBlendEnable = FALSE;												//���������_�[�^�[�Q�b�g�œƗ������u�����f�B���O��L���ɂ��邩�ǂ���
	blend_desc.RenderTarget[0].BlendEnable = TRUE;											//�u�����f�B���O�̗L��
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;							//�s�N�Z���V�F�[�_�[���o�͂���RGB�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;								//�����_�[�^�[�Q�b�g�̌��݂�RGB�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;								//����������@���`��SrcBlend��DestBlend����𐧌�
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;								//�s�N�Z���V�F�[�_�[���o�͂���A���t�@�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;							//�����_�[�^�[�Q�b�g�̌��݂̃A���t�@�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;							//�ǂ̂悤�ɑg�ݍ��킹�邩���`��SrcBlendAlpha��DestBlendAlpha����𐧌�
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;		//�������݃}�X�N

	hr = device->CreateBlendState(
		&blend_desc,						//�f�X�N�̃A�h���X
		blend_states[4].GetAddressOf()		//�쐬���ꂽ�u�����f�B���O�X�e�[�g�I�u�W�F�N�g�̃A�h���X
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//�u�����h���[�h : MULTIPLY
	blend_desc.AlphaToCoverageEnable = FALSE;												//�s�N�Z���������_�[�^�[�Q�b�g�ɐݒ肷��Ƃ��A�A���t�@����J�o���b�W���}���`�T���v�����O��@�Ƃ��Ďg�p���邩�ǂ���
	blend_desc.IndependentBlendEnable = FALSE;												//���������_�[�^�[�Q�b�g�œƗ������u�����f�B���O��L���ɂ��邩�ǂ���
	blend_desc.RenderTarget[0].BlendEnable = TRUE;											//�u�����f�B���O�̗L��
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;									//�s�N�Z���V�F�[�_�[���o�͂���RGB�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_COLOR;							//�����_�[�^�[�Q�b�g�̌��݂�RGB�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;								//����������@���`��SrcBlend��DestBlend����𐧌�
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_DEST_ALPHA;						//�s�N�Z���V�F�[�_�[���o�͂���A���t�@�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;							//�����_�[�^�[�Q�b�g�̌��݂̃A���t�@�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;							//�ǂ̂悤�ɑg�ݍ��킹�邩���`��SrcBlendAlpha��DestBlendAlpha����𐧌�
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;		//�������݃}�X�N

	hr = device->CreateBlendState(
		&blend_desc,						//�f�X�N�̃A�h���X
		blend_states[5].GetAddressOf()		//�쐬���ꂽ�u�����f�B���O�X�e�[�g�I�u�W�F�N�g�̃A�h���X
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//�u�����h���[�h : LIGHTEN
	blend_desc.AlphaToCoverageEnable = FALSE;												//�s�N�Z���������_�[�^�[�Q�b�g�ɐݒ肷��Ƃ��A�A���t�@����J�o���b�W���}���`�T���v�����O��@�Ƃ��Ďg�p���邩�ǂ���
	blend_desc.IndependentBlendEnable = FALSE;												//���������_�[�^�[�Q�b�g�œƗ������u�����f�B���O��L���ɂ��邩�ǂ���
	blend_desc.RenderTarget[0].BlendEnable = TRUE;											//�u�����f�B���O�̗L��
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;									//�s�N�Z���V�F�[�_�[���o�͂���RGB�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;									//�����_�[�^�[�Q�b�g�̌��݂�RGB�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MAX;								//����������@���`��SrcBlend��DestBlend����𐧌�
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;								//�s�N�Z���V�F�[�_�[���o�͂���A���t�@�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;							//�����_�[�^�[�Q�b�g�̌��݂̃A���t�@�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;							//�ǂ̂悤�ɑg�ݍ��킹�邩���`��SrcBlendAlpha��DestBlendAlpha����𐧌�
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;		//�������݃}�X�N

	hr = device->CreateBlendState(
		&blend_desc,						//�f�X�N�̃A�h���X
		blend_states[6].GetAddressOf()		//�쐬���ꂽ�u�����f�B���O�X�e�[�g�I�u�W�F�N�g�̃A�h���X
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//�u�����h���[�h : DARKEN
	blend_desc.AlphaToCoverageEnable = FALSE;												//�s�N�Z���������_�[�^�[�Q�b�g�ɐݒ肷��Ƃ��A�A���t�@����J�o���b�W���}���`�T���v�����O��@�Ƃ��Ďg�p���邩�ǂ���
	blend_desc.IndependentBlendEnable = FALSE;												//���������_�[�^�[�Q�b�g�œƗ������u�����f�B���O��L���ɂ��邩�ǂ���
	blend_desc.RenderTarget[0].BlendEnable = TRUE;											//�u�����f�B���O�̗L��
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;									//�s�N�Z���V�F�[�_�[���o�͂���RGB�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;									//�����_�[�^�[�Q�b�g�̌��݂�RGB�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MIN;								//����������@���`��SrcBlend��DestBlend����𐧌�
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;								//�s�N�Z���V�F�[�_�[���o�͂���A���t�@�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;							//�����_�[�^�[�Q�b�g�̌��݂̃A���t�@�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MIN;							//�ǂ̂悤�ɑg�ݍ��킹�邩���`��SrcBlendAlpha��DestBlendAlpha����𐧌�
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;		//�������݃}�X�N

	hr = device->CreateBlendState(
		&blend_desc,						//�f�X�N�̃A�h���X
		blend_states[7].GetAddressOf()		//�쐬���ꂽ�u�����f�B���O�X�e�[�g�I�u�W�F�N�g�̃A�h���X
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//�u�����h���[�h : SCREEN
	blend_desc.AlphaToCoverageEnable = FALSE;												//�s�N�Z���������_�[�^�[�Q�b�g�ɐݒ肷��Ƃ��A�A���t�@����J�o���b�W���}���`�T���v�����O��@�Ƃ��Ďg�p���邩�ǂ���
	blend_desc.IndependentBlendEnable = FALSE;												//���������_�[�^�[�Q�b�g�œƗ������u�����f�B���O��L���ɂ��邩�ǂ���
	blend_desc.RenderTarget[0].BlendEnable = TRUE;											//�u�����f�B���O�̗L��
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;							//�s�N�Z���V�F�[�_�[���o�͂���RGB�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;						//�����_�[�^�[�Q�b�g�̌��݂�RGB�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;								//����������@���`��SrcBlend��DestBlend����𐧌�
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;								//�s�N�Z���V�F�[�_�[���o�͂���A���t�@�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;					//�����_�[�^�[�Q�b�g�̌��݂̃A���t�@�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;							//�ǂ̂悤�ɑg�ݍ��킹�邩���`��SrcBlendAlpha��DestBlendAlpha����𐧌�
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;		//�������݃}�X�N

	hr = device->CreateBlendState(
		&blend_desc,						//�f�X�N�̃A�h���X
		blend_states[8].GetAddressOf()		//�쐬���ꂽ�u�����f�B���O�X�e�[�g�I�u�W�F�N�g�̃A�h���X
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//�u�����h���[�h : COVERAGE
	blend_desc.AlphaToCoverageEnable = TRUE;												//�s�N�Z���������_�[�^�[�Q�b�g�ɐݒ肷��Ƃ��A�A���t�@����J�o���b�W���}���`�T���v�����O��@�Ƃ��Ďg�p���邩�ǂ���
	blend_desc.IndependentBlendEnable = FALSE;												//���������_�[�^�[�Q�b�g�œƗ������u�����f�B���O��L���ɂ��邩�ǂ���
	blend_desc.RenderTarget[0].BlendEnable = TRUE;											//�u�����f�B���O�̗L��
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;							//�s�N�Z���V�F�[�_�[���o�͂���RGB�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;						//�����_�[�^�[�Q�b�g�̌��݂�RGB�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;								//����������@���`��SrcBlend��DestBlend����𐧌�
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;								//�s�N�Z���V�F�[�_�[���o�͂���A���t�@�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;					//�����_�[�^�[�Q�b�g�̌��݂̃A���t�@�l�ɑ΂��Ď��s���鑀����w��
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;							//�ǂ̂悤�ɑg�ݍ��킹�邩���`��SrcBlendAlpha��DestBlendAlpha����𐧌�
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;		//�������݃}�X�N

	hr = device->CreateBlendState(
		&blend_desc,						//�f�X�N�̃A�h���X
		blend_states[9].GetAddressOf()		//�쐬���ꂽ�u�����f�B���O�X�e�[�g�I�u�W�F�N�g�̃A�h���X
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	/*************�����܂�**************/


	/*************���X�^���C�U�[�X�e�[�g�I�u�W�F�N�g�̐���**************/

	D3D11_RASTERIZER_DESC rasterizer_desc{};
	rasterizer_desc.FillMode = D3D11_FILL_SOLID;		//�����_�����O���Ɏg�p����h��Ԃ����[�h
	rasterizer_desc.CullMode = D3D11_CULL_BACK;			//�w�肳�ꂽ�����������Ă���O�p�`���`�悳��Ă��Ȃ����Ƃ�����
	rasterizer_desc.FrontCounterClockwise = TRUE;		//�O�p�`���O�ʂ��w�ʂ�������
	rasterizer_desc.DepthBias = 0;						//����̃s�N�Z���ɒǉ����ꂽ�[�x�l
	rasterizer_desc.DepthBiasClamp = 0;					//�s�N�Z���̍ő�[�x
	rasterizer_desc.SlopeScaledDepthBias = 0;			//����̃s�N�Z���̌X�Ώ�̃X�J���[
	rasterizer_desc.DepthClipEnable = TRUE;				//�����Ɋ�Â��ăN���b�s���O��L���ɂ���
	rasterizer_desc.ScissorEnable = FALSE;				//�����`�̃J�����O��L���ɂ���
	rasterizer_desc.MultisampleEnable = FALSE;			//�}���`�T���v���A���`�G�C���A�V���O
	rasterizer_desc.AntialiasedLineEnable = FALSE;		//����A���`�G�C���A�X

	//�h��Ԃ�/���ʃJ�����O
	hr = device->CreateRasterizerState(&rasterizer_desc, rasterizer_state[0].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//�h��Ԃ�/�J�����O�Ȃ�
	rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	rasterizer_desc.CullMode = D3D11_CULL_NONE;
	rasterizer_desc.AntialiasedLineEnable = FALSE;

	hr = device->CreateRasterizerState(&rasterizer_desc, rasterizer_state[1].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//���C���[�t���[��/���ʃJ�����O
	rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizer_desc.CullMode = D3D11_CULL_BACK;
	rasterizer_desc.AntialiasedLineEnable = TRUE;

	hr = device->CreateRasterizerState(&rasterizer_desc, rasterizer_state[2].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//���C���[�t���[��/�J�����O�Ȃ�
	rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizer_desc.CullMode = D3D11_CULL_NONE;
	rasterizer_desc.AntialiasedLineEnable = TRUE;

	hr = device->CreateRasterizerState(&rasterizer_desc, rasterizer_state[3].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	/*************�����܂�**************/
}