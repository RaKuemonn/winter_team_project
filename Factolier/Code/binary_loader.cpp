
#include "Binary_Loader.h"
#include <fstream>


void File_IO::open(const char* filename, Binary_Data& data_)
{
    std::ifstream fin(filename, std::ios::in | std::ios::binary);

	if (!fin) return;

    int value;

	while (true)
	{
		fin.read((char*)&value, sizeof(int));

		if (fin.eof()) break;

		data_.data_array.emplace_back(value);
	}

	fin.close();
}


void File_IO::write(const char* filename, const Binary_Data& data_)
{
	std::ofstream fout;
	fout.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);

	if (!fout) return;

	for (size_t i = 0; i < data_.data_array.size(); ++i)
	{
		fout.write((char*)&data_.data_array.at(i), sizeof(int));
	}

	fout.close();
}