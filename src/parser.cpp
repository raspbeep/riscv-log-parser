#include <iostream>
#include <cmath>

#include "reader.hpp"
#include "line_parse.hpp"
#include "instructions.hpp"
#include "decoder.hpp"

void print_progress_bar(float progress)
{
    int barWidth = 50; // Width of the progress bar
    std::cerr << "\r[";
    int pos = static_cast<int>(barWidth * progress);
    for (int i = 0; i < barWidth; ++i)
    {
        if (i < pos)
            std::cerr << "=";
        else if (i == pos)
            std::cerr << ">";
        else
            std::cerr << " ";
    }
    std::cerr << "] " << int(progress * 100.0) << " %";
    std::cerr.flush();
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <log_file_path>\n";
        return 1;
    }
    std::string file_name = argv[1];

    auto file_reader = FileReader(file_name);
    std::string line;
    uint32_t code;
    size_t count_compressed = 0;
    size_t count = 0;

    size_t total_lines = file_reader.get_lines_count() + 1;

    file_reader.open(file_name);

    while (file_reader.get_next_line(line))
    {
        DecodedInstruction inst{};
        code = extract_instruction_from_line(line);
        inst.line = line;
        decode_instruction(code, inst);

        std::cout << inst.line << std::endl;
        std::cout << inst.mnemonic << " ";
        inst.print_payload();
        std::cout << std::endl;

        if (inst.compressed)
            count_compressed++;
        count++;

        if (count % 100000 == 0)
        {
            // Show loading bar
            float progress = (float)count / total_lines;
            print_progress_bar(progress);
        }
    }

    std::cerr << std::endl;
    std::cerr << "compressed: " << count_compressed << std::endl;
    std::cerr << "all: " << count << std::endl;

    return 0;
}
