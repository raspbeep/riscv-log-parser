#include "utils.hpp"

// Convert uint32_t to hexadecimal string
std::string uint32_t_to_hex(uint32_t value)
{
    std::stringstream ss;
    ss << std::hex << std::uppercase << value;
    return ss.str();
}

// Convert uint32_t to decimal string
std::string uint32_t_to_dec(uint32_t value)
{
    return std::to_string(value);
}

// Convert uint32_t to binary string
std::string uint32_t_to_bin(uint32_t value)
{
    std::bitset<32> b(value);
    return b.to_string();
}

std::string uint32_t_to_dec_hex_bin(uint32_t value)
{
    std::stringstream ss;
    ss << "bin: " << uint32_t_to_bin(value) << " (dec: " << uint32_t_to_dec(value) << ") " << "(hex: 0x" << uint32_t_to_hex(value) << ")";
    return ss.str();
}

int32_t sign_extend(uint32_t value, unsigned int bits)
{
    uint32_t mask = 1U << (bits - 1);
    return (int32_t)((value ^ mask) - mask);
}