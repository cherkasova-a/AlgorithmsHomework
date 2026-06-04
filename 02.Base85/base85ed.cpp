#include <vector>
#include <cstdint>
#include <string>
#include <stdexcept>

#include "base85ed.h"

static const char B85_ALPHABET[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "!#$%&()*+-;<=>?@^_`{|}~";

static int b85_decode_char(uint8_t c)
{
    static int table[256] = {};
    static bool init = false;
    if (!init)
    {
        for (int i = 0; i < 256; ++i) table[i] = -1;
        for (int i = 0; i < 85; ++i) table[(uint8_t)B85_ALPHABET[i]] = i;
        init = true;
    }
    return table[c];
}

std::vector<uint8_t> base85::encode(std::vector<uint8_t> const &bytes)
{
    std::vector<uint8_t> result;
    size_t i = 0;

    for (; i + 4 <= bytes.size(); i += 4)
    {
        uint32_t num = ((uint32_t)bytes[i] << 24)
                       | ((uint32_t)bytes[i + 1] << 16)
                       | ((uint32_t)bytes[i + 2] << 8)
                       | (uint32_t)bytes[i + 3];
        char block[5];
        for (int j = 4; j >= 0; --j)
        {
            block[j] = B85_ALPHABET[num % 85];
            num /= 85;
        }
        result.insert(result.end(), block, block + 5);
    }

    size_t remaining = bytes.size() - i;
    if (remaining > 0)
    {
        uint32_t num = 0;
        for (size_t j = 0; j < 4; ++j)
        {
            num <<= 8;
            if (j < remaining) num |= bytes[i + j];
        }
        char block[5];
        for (int j = 4; j >= 0; --j)
        {
            block[j] = B85_ALPHABET[num % 85];
            num /= 85;
        }
        result.insert(result.end(), block, block + remaining + 1);
    }

    return result;
}

std::vector<uint8_t> base85::decode(std::vector<uint8_t> const &b85str)
{
    std::vector<uint8_t> result;
    size_t len = b85str.size();
    size_t i = 0;

    while (i < len)
    {
        if (i + 5 <= len)
        {
            uint32_t num = 0;
            for (int j = 0; j < 5; ++j)
            {
                int v = b85_decode_char(b85str[i + j]);
                if (v < 0)
                    throw std::runtime_error(
                        std::string("bad base85 character at position ") + std::to_string(i + j));
                num = num * 85 + v;
            }
            result.push_back((num >> 24) & 0xFF);
            result.push_back((num >> 16) & 0xFF);
            result.push_back((num >> 8) & 0xFF);
            result.push_back(num & 0xFF);
            i += 5;
        }
        else
        {
            size_t chunk = len - i;
            uint32_t num = 0;
            for (size_t j = 0; j < 5; ++j)
            {
                int v;
                if (j < chunk)
                {
                    v = b85_decode_char(b85str[i + j]);
                    if (v < 0)
                        throw std::runtime_error(
                            std::string("bad base85 character at position ") + std::to_string(i + j));
                }
                else
                {
                    v = 84;
                }
                num = num * 85 + v;
            }
            for (size_t j = 0; j < chunk - 1; ++j)
            {
                result.push_back((num >> (24 - j * 8)) & 0xFF);
            }
            i = len;
        }
    }

    return result;
}
