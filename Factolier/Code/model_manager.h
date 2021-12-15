#pragma once

#include <memory>
#include <string>
#include <map>
#include "model_resource.h"


//リソースマネージャー
class Model_Manager
{
public:
	//コンストラクタ
	Model_Manager(ID3D11Device* device) { _device = device; }


	//デストラクタ
	~Model_Manager() {}


	//モデルリソース読み込み
	std::shared_ptr<Model_Resource> load_model(const char* filename, bool triangulate = false, float rate = 60.0f);

	
	void load();
	void release();

private:
	using ModelMap = std::map<std::string, std::shared_ptr<Model_Resource>>;
	ID3D11Device* _device;

	ModelMap resources;
};
