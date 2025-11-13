#include "reader.hpp"
#include "line_parse.hpp"
#include "instructions.hpp"
#include "decoder.hpp"

#include <iostream>

#define FIRST_N 200

int main()
{
    auto file_reader = FileReader("compressed_only.log");
    std::string line;
    uint32_t code;
    BaseInstruction inst;
    int count = 0;
    while (file_reader.get_next_line(line) && count++ < FIRST_N)
    {
        code = extract_instruction_from_line(line);
        inst = decode_instruction(code);
        std::cout << " " << (inst.compressed ? "compressed" : "uncompressed") << std::endl;
    }

    return 0;
}