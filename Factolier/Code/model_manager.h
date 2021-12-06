#pragma once

#include <memory>
#include <string>
#include <map>
#include "skinned_mesh.h"


//���\�[�X�}�l�[�W���[
class Model_Manager
{
public:
	//�R���X�g���N�^
	Model_Manager(ID3D11Device* device) { _device = device; }


	//�f�X�g���N�^
	~Model_Manager() {}


	//���f�����\�[�X�ǂݍ���
	std::shared_ptr<Skinned_Mesh> load_model(const char* filename, bool triangulate = false, float rate = 60.0f);

	
	void load();
	void release();

private:
	using ModelMap = std::map<std::string, std::shared_ptr<Skinned_Mesh>>;
	ID3D11Device* _device;

	ModelMap resources;
};
