#include <iostream>

#include "reader.hpp"
#include "line_parse.hpp"
#include "instructions.hpp"
#include "decoder.hpp"

int main()
{
    auto file_reader = FileReader("logs/commit.log");
    std::string line;
    uint32_t code;
    size_t count_compressed = 0;
    size_t count = 0;
    while (file_reader.get_next_line(line))
    {
        DecodedInstruction inst;
        code = extract_instruction_from_line(line);
        inst.line = line;
        decode_instruction(code, inst);

        if (inst.compressed)
            count_compressed++;
        count++;
    }

    std::cout << "compressed: " << count_compressed << std::endl;
    std::cout << "all: " << count << std::endl;

    return 0;
}
