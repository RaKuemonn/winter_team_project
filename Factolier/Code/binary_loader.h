#pragma once

#include <vector>


struct Binary_Data;

class File_IO
{
public:
    static void open(const char* filename, Binary_Data& data_);

    static void write(const char* filename, const Binary_Data& data_);

};


struct Binary_Data
{
    std::vector<int> data_array = {};

    void set_data(const int data_)
    {
        data_array.emplace_back(data_);
    }

    size_t size()
    {
        return data_array.size();
    }

};
