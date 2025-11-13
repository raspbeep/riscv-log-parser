#ifndef LINE_PARSE_HPP
#define LINE_PARSE_HPP

#include <string>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <algorithm>

uint32_t extract_instruction_from_line(const std::string &line);

#endif