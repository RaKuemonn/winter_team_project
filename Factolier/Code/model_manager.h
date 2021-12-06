#pragma once

#include <memory>
#include <string>
#include <map>
#include "skinned_mesh.h"


//リソースマネージャー
class Model_Manager
{
public:
	//コンストラクタ
	Model_Manager(ID3D11Device* device) { _device = device; }


	//デストラクタ
	~Model_Manager() {}


	//モデルリソース読み込み
	std::shared_ptr<Skinned_Mesh> load_model(const char* filename, bool triangulate = false, float rate = 60.0f);

	
	void load();
	void release();

private:
	using ModelMap = std::map<std::string, std::shared_ptr<Skinned_Mesh>>;
	ID3D11Device* _device;

	ModelMap resources;
};
