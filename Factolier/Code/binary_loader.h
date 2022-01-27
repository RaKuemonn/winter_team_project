#pragma once

#include <fstream>

struct Binary_Data;

template<typename T>
class File_IO
{
public:
    static void open(const char* filename, T& data_, bool deserialize = true)
	{
		if (deserialize)
		{
			std::ifstream fin(filename, std::ios::in | std::ios::binary);

			if (!fin) return;

			fin.read((char*)&data_, sizeof(T));

			fin.close();
		}

		else
		{
			std::ifstream fin(filename, std::ios::in);

			if (!fin) return;

			fin.read((char*)&data_, sizeof(T));

			fin.close();
		}
	}

	static void write(const char* filename, const T& data_, bool serialize = true)
	{
		if (serialize)
		{
			std::ofstream fout;
			fout.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);

			if (!fout) return;

			fout.write((char*)&data_, sizeof(T));

			fout.close();
		}
		else
		{
			std::ofstream fout;
			fout.open(filename, std::ios::out | std::ios::trunc);

			if (!fout) return;

			fout.write((char*)&data_, sizeof(T));

			fout.close();
		}

	}


	static void write(const char* filename, const std::vector<DirectX::XMFLOAT3>& data_, int i)
	{
		std::ofstream fout;
		fout.open(filename, std::ios::out | std::ios::trunc);

		if (!fout) return;

		std::string data = {};
		for (auto& dat : data_)
		{
			data +=
				"{" + std::to_string(dat.x) +
				"," + std::to_string(dat.y) +
				"," + std::to_string(dat.z) +
				"}\n";
		}

		fout.write(data.data(), sizeof(char) * data.size());

		fout.close();

	}
};


struct Binary_Data
{
    bool clear_flag[5]{ false, false, false, false, false };
    float bgm_bar = 126.0f;
    float se_bar = 126.0f;
    float camera_bar = 252.0f;
};
