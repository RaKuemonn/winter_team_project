
#include "model_manager.h"


//モデルリソース読み込み
std::shared_ptr<Model_Resource> Model_Manager::load_model(const char* filename, bool triangulate, float rate)
{
    //リソースを検索
    std::shared_ptr<Model_Resource> resource;

    auto it = resources.find(filename);

    if (it != resources.end())
    {
        resource = it->second;

        return resource;
    }

    resource = std::make_shared<Model_Resource>(_device, filename, triangulate, rate);
    resources.emplace(filename, resource);


    return resource;
}


void Model_Manager::load()
{
    
}


void Model_Manager::release()
{
    
}

