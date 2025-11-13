#ifndef FILE_READER_HPP
#define FILE_READER_HPP

#include <fstream>

class FileReader
{
private:
    std::ifstream m_ifile;

public:
    FileReader(const std::string &ifile_name);

    ~FileReader();

    void open(const std::string &ifile_name);

    void close();

    bool get_next_line(std::string &line);
};

#endif