#include <iostream>

#include "reader.hpp"

FileReader::FileReader(const std::string &ifile_name)
{
    open(ifile_name);
}

FileReader::~FileReader()
{
    close();
}

void FileReader::open(const std::string &ifile_name)
{
    if (m_ifile.is_open())
    {
        m_ifile.close();
    }
    m_ifile.open(ifile_name);
    if (!m_ifile.is_open())
    {
        throw std::runtime_error("Could not open file: " + ifile_name);
    }
}

void FileReader::close()
{
    if (m_ifile.is_open())
    {
        m_ifile.close();
    }
}

bool FileReader::get_next_line(std::string &line)
{
    if (!m_ifile.is_open())
    {
        throw std::runtime_error("Could not read file");
    }

    if (std::getline(m_ifile, line))
    {
        return true;
    }
    return false;
}

std::size_t FileReader::get_lines_count()
{
    if (!m_ifile.is_open())
    {
        throw std::runtime_error("File is not open");
    }

    std::streampos curr_pos = m_ifile.tellg();
    m_ifile.clear();

    m_ifile.seekg(0, std::ios::beg);

    std::size_t line_count = 0;
    std::string tmp;
    while (std::getline(m_ifile, tmp))
    {
        ++line_count;
    }

    m_ifile.clear();
    m_ifile.seekg(curr_pos);

    return line_count;
}
