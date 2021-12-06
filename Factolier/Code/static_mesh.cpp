
#include "static_mesh.h"
#include "shader.h"
#include "texture.h"


Static_Mesh::Static_Mesh(ID3D11Device* device, const wchar_t* obj_filename, bool vt_sub)
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    uint32_t current_index{ 0 };

    std::vector<DirectX::XMFLOAT3> positions;
    std::vector<DirectX::XMFLOAT3> normals;
    std::vector<DirectX::XMFLOAT2> texcoords;
    std::vector<wstring> mtl_filenames;

    //OBJファイルパーサー
    std::wifstream fin(obj_filename);
    _ASSERT_EXPR(fin, L"'OBJ file  not found.");
    wchar_t command[256];
    while (fin)
    {
        fin >> command;

        //頂点座標
        if (0 == wcscmp(command, L"v"))
        {
            float x, y, z;
            fin >> x >> y >> z;
            positions.emplace_back(DirectX::XMFLOAT3{ x, y, z });
            fin.ignore(1024, L'\n');
        }

        //法線
        else if (0 == wcscmp(command, L"vn"))
        {
            FLOAT i, j, k;
            fin >> i >> j >> k;
            normals.emplace_back(DirectX::XMFLOAT3{ i, j, k });
            fin.ignore(1024, L'\n');
        }

        //テクスチャ座標
        else if (0 == wcscmp(command, L"vt"))
        {
            float u, v;
            fin >> u >> v;
            if (vt_sub) texcoords.emplace_back(DirectX::XMFLOAT2{ u, 1.0f - v });
            else texcoords.emplace_back(DirectX::XMFLOAT2{ u, v });
            fin.ignore(1024, L'\n');
        }

        //ポリゴン宣言
        else if (0 == wcscmp(command, L"f"))
        {
            for (size_t i = 0; i < 3; i++)
            {
                Vertex vertex;
                size_t v, vt, vn;

                fin >> v;
                vertex.position = positions.at(v - 1);
                if (L'/' == fin.peek())
                {
                    fin.ignore(1);
                    if (L'/' != fin.peek())
                    {
                        fin >> vt;
                        vertex.texcoord = texcoords.at(vt - 1);
                    }

                    if (L'/' == fin.peek())
                    {
                        fin.ignore(1);
                        fin >> vn;
                        vertex.normal = normals.at(vn - 1);
                    }
                }

                vertices.push_back(vertex);
                indices.push_back(current_index++);
            }

            fin.ignore(1024, L'\n');
        }

        else if (0 == wcscmp(command, L"mtllib"))
        {
            wchar_t mtllib[256];
            fin >> mtllib;
            mtl_filenames.emplace_back(mtllib);
        }

        else if (0 == wcscmp(command, L"usemtl"))
        {
            wchar_t usemtl[MAX_PATH]{ 0 };
            fin >> usemtl;
            subsets.emplace_back(Subset{ usemtl, static_cast<uint32_t>(indices.size()), 0 });
        }

        else
        {
            fin.ignore(1024, L'\n');
        }
    }

    fin.close();

    //サブセットのインデックス数を計算
    std::vector<Subset>::reverse_iterator iterator = subsets.rbegin();
    iterator->index_count = static_cast<uint32_t>(indices.size()) - iterator->index_start;

    for (iterator = subsets.rbegin() + 1; iterator < subsets.rend(); ++iterator)
    {
        iterator->index_count = (iterator - 1)->index_start - iterator->index_start;
    }


    //mtlファイルパーサー
    std::filesystem::path mtl_filename(obj_filename);
    mtl_filename.replace_filename(std::filesystem::path(mtl_filenames[0]).filename());

    fin.open(mtl_filename);
    //mtlがない場合、ダミーマテリアルを作成
    if (materials.size() == 0)
    {
        for (const Subset& subset : subsets)
        {
            materials.push_back({ subset.usemtl });
        }
    }


    while (fin)
    {
        fin >> command;

        //テクスチャ名
        if (0 == wcscmp(command, L"map_Kd"))
        {
            fin.ignore();
            wchar_t map_Kd[256];
            fin >> map_Kd;

            std::filesystem::path path(obj_filename);
            path.replace_filename(std::filesystem::path(map_Kd).filename());
            materials.rbegin()->texture_filename[0] = path.string();
            fin.ignore(1024, L'\n');
        }

        //バンプマップ処理
        else if (0 == wcscmp(command, L"map_bump") || 0 == wcscmp(command, L"bump"))
        {
            fin.ignore();
            wchar_t map_bump[256];
            fin >> map_bump;
            std::filesystem::path path(obj_filename);
            path.replace_filename(std::filesystem::path(map_bump).filename());
            materials.rbegin()->texture_filename[1] = path.string();
            fin.ignore(1024, L'\n');
        }

        //マテリアル名宣言
        else if (0 == wcscmp(command, L"newmtl"))
        {
            fin.ignore();
            wchar_t newmtl[256];
            Material material;
            fin >> newmtl;
            material.name = newmtl;
            materials.emplace_back(material);
        }

        //ディフューズカラー
        else if (0 == wcscmp(command, L"Kd"))
        {
            float r, g, b;
            fin >> r >> g >> b;
            materials.rbegin()->Kd = { r, g, b, 1 };
            fin.ignore(1024, L'\n');
        }

        //アンビエントカラー
        else if (0 == wcscmp(command, L"Ka"))
        {
            float r, g, b;
            fin >> r >> g >> b;
            materials.rbegin()->Ka = { r, g, b, 1 };
            fin.ignore(1024, L'\n');
        }

        //スペキュラーカラー
        else if (0 == wcscmp(command, L"Ks"))
        {
            float r, g, b;
            fin >> r >> g >> b;
            materials.rbegin()->Ks = { r, g, b, 1 };
            fin.ignore(1024, L'\n');
        }

        else
        {
            fin.ignore(1024, L'\n');
        }
    }
    fin.close();


    //バッファの生成
    create_com_buffers(device, vertices.data(), vertices.size(), indices.data(), indices.size());

    HRESULT hr{ S_OK };

    D3D11_INPUT_ELEMENT_DESC input_element_desc[]
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
        D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
        D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
        D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };


    create_vs_from_cso(device, "./CSO/static_mesh_vs.cso", vertex_shader.GetAddressOf(),
        input_layout.GetAddressOf(), input_element_desc, ARRAYSIZE(input_element_desc));
    create_ps_from_cso(device, "./CSO/static_mesh_ps.cso", pixel_shader.GetAddressOf());


    D3D11_BUFFER_DESC buffer_desc{};
    buffer_desc.ByteWidth = sizeof(Constants);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    hr = device->CreateBuffer(&buffer_desc, nullptr, constant_buffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


    //シェーダーリソースビューの生成
    D3D11_TEXTURE2D_DESC texture2d_desc{};

    for (Material& material : materials)
    {
        if (material.texture_filename[0].size() == 0) make_dummy_texture(device, material.shader_resource_view[0].GetAddressOf(), 0xFFFFFFFF, 16);    //ダミーカラーマップテクスチャの作成
        else load_texture_from_file(device, material.texture_filename[0].c_str(), material.shader_resource_view[0].GetAddressOf(), &texture2d_desc);


        if (material.texture_filename[1].size() == 0) make_dummy_texture(device, material.shader_resource_view[1].GetAddressOf(), 0xFFFF7F7F, 16);    //ダミー法線マップテクスチャの作成
        else load_texture_from_file(device, material.texture_filename[1].c_str(), material.shader_resource_view[1].GetAddressOf(), &texture2d_desc);
    }
}


void Static_Mesh::create_com_buffers(ID3D11Device* device, Vertex* vertices, size_t vertex_count, uint32_t* indices, size_t index_count)
{
    HRESULT hr{ S_OK };

    D3D11_BUFFER_DESC buffer_desc{};
    D3D11_SUBRESOURCE_DATA subresource_data{};
    buffer_desc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * vertex_count);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    buffer_desc.CPUAccessFlags = 0;
    buffer_desc.MiscFlags = 0;
    buffer_desc.StructureByteStride = 0;
    subresource_data.pSysMem = vertices;
    subresource_data.SysMemPitch = 0;
    subresource_data.SysMemSlicePitch = 0;

    hr = device->CreateBuffer(&buffer_desc, &subresource_data, vertex_buffer.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


    buffer_desc.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * index_count);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    subresource_data.pSysMem = indices;

    hr = device->CreateBuffer(&buffer_desc, &subresource_data, index_buffer.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}


HRESULT Static_Mesh::make_dummy_texture(ID3D11Device* device, ID3D11ShaderResourceView** shader_resource_view, DWORD value/*0xAABBGGRR*/, UINT dimension)
{
    HRESULT hr{ S_OK };

    D3D11_TEXTURE2D_DESC texture2d_desc{};
    texture2d_desc.Width = dimension;
    texture2d_desc.Height = dimension;
    texture2d_desc.MipLevels = 1;
    texture2d_desc.ArraySize = 1;
    texture2d_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texture2d_desc.SampleDesc.Count = 1;
    texture2d_desc.SampleDesc.Quality = 0;
    texture2d_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;


    size_t texels = dimension * dimension;
    unique_ptr<DWORD[]> sysmem{ make_unique<DWORD[]>(texels) };
    for (size_t i = 0; i < texels; ++i) sysmem[i] = value;


    D3D11_SUBRESOURCE_DATA subresource_data{};
    subresource_data.pSysMem = sysmem.get();
    subresource_data.SysMemPitch = sizeof(DWORD) * dimension;


    ComPtr<ID3D11Texture2D> texture2d;
    hr = device->CreateTexture2D(&texture2d_desc, &subresource_data, &texture2d);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


    D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc{};
    shader_resource_view_desc.Format = texture2d_desc.Format;
    shader_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shader_resource_view_desc.Texture2D.MipLevels = 1;

    hr = device->CreateShaderResourceView(texture2d.Get(), &shader_resource_view_desc, shader_resource_view);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


    return hr;
}


void Static_Mesh::render(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color)
{
    uint32_t stride{ sizeof(Vertex) };
    uint32_t offset{ 0 };

    immediate_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
    immediate_context->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    immediate_context->IASetInputLayout(input_layout.Get());

    immediate_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
    immediate_context->PSSetShader(pixel_shader.Get(), nullptr, 0);

    for (const Material& material : materials)
    {
        immediate_context->PSSetShaderResources(0, 1, material.shader_resource_view[0].GetAddressOf());
        immediate_context->PSSetShaderResources(1, 1, material.shader_resource_view[1].GetAddressOf());

        Constants data{ world, material_color };
        immediate_context->UpdateSubresource(constant_buffer.Get(), 0, 0, &data, 0, 0);
        immediate_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
        immediate_context->PSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());

        for (const Subset& subset : subsets)
        {
            if (material.name == subset.usemtl)
            {
                immediate_context->DrawIndexed(subset.index_count, subset.index_start, 0);
            }
        }
    }
}
