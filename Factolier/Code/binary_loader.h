#pragma once


struct Binary_Data;

class File_IO
{
public:
    static void open(const char* filename, Binary_Data& data_);

    static void write(const char* filename, const Binary_Data& data_);

};


struct Binary_Data
{
    bool clear_flag[4]{ false, false, false, false };
};
