
#include "model_manager.h"


//���f�����\�[�X�ǂݍ���
std::shared_ptr<Skinned_Mesh> Model_Manager::load_model(const char* filename, bool triangulate, float rate)
{
    //���\�[�X������
    std::shared_ptr<Skinned_Mesh> resource;

    auto it = resources.find(filename);

    if (it != resources.end())
    {
        resource = it->second;

        return resource;
    }

    resource = std::make_shared<Skinned_Mesh>(_device, filename, triangulate, rate);
    resources.emplace(filename, resource);


    return resource;
}


void Model_Manager::load()
{
    
}


void Model_Manager::release()
{
    
}

