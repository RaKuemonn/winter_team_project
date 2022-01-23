
#include "Binary_Loader.h"
#include <fstream>


void File_IO::open(const char* filename, Binary_Data& data_)
{
    std::ifstream fin(filename, std::ios::in | std::ios::binary);

	if (!fin) return;

	fin.read((char*)&data_, sizeof(Binary_Data));

	fin.close();
}


void File_IO::write(const char* filename, const Binary_Data& data_)
{
	std::ofstream fout;
	fout.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);

	if (!fout) return;

	fout.write((char*)&data_, sizeof(Binary_Data));

	fout.close();
}