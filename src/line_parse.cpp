#ifndef LINE_PARSER_HPP
#define LINE_PARSER_HPP

#include "line_parse.hpp"

uint32_t extract_instruction_from_line(const std::string &line)
{
    size_t leftParen = line.find('(');
    size_t rightParen = line.find(')');
    if (leftParen == std::string::npos || rightParen == std::string::npos || rightParen <= leftParen)
        throw std::runtime_error("Cannot find instruction code in input line.");

    std::string hexStr = line.substr(leftParen + 1, rightParen - leftParen - 1); // e.g., "0x07a1"
    uint32_t code = 0;

    // Remove possible whitespace
    hexStr.erase(remove_if(hexStr.begin(), hexStr.end(), isspace), hexStr.end());

    std::stringstream ss(hexStr);
    if (hexStr.compare(0, 2, "0x") == 0 || hexStr.compare(0, 2, "0X") == 0)
    {
        ss >> std::hex >> code;
    }
    else
    {
        ss >> code;
    }
    return code;
}

#endif // LINE_PARSER_HPP