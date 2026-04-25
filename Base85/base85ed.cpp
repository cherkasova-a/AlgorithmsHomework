#include "base85ed.h"
#include <stdexcept>
#include <array>
#include <algorithm>
#include <cstring>

namespace base85 {

static const char* ENCODE_CHARS = 
    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!#$%&()*+-;<=>?@^_`{|}~";

std::vector<uint8_t> encode(const std::vector<uint8_t>& bytes) {
    std::vector<uint8_t> result;
    size_t n = bytes.size();
    
    for (size_t i = 0; i < n; i += 4) {
        uint32_t val = 0;
        int padding = 0;
        
        for (int j = 0; j < 4; ++j) {
            val <<= 8;
            if (i + j < n) {
                val |= bytes[i + j];
            } else {
                padding++;
            }
        }
        
        std::array<uint8_t, 5> block;
        for (int j = 4; j >= 0; --j) {
            block[j] = ENCODE_CHARS[val % 85];
            val /= 85;
        }
        
        for (int j = 0; j < 5 - padding; ++j) {
            result.push_back(block[j]);
        }
    }
    return result;
}

std::vector<uint8_t> decode(const std::vector<uint8_t>& b85str) {
    std::vector<uint8_t> result;
    size_t n = b85str.size();
    
    auto char_to_val = [](char c) -> uint32_t {
        const char* p = std::strchr(ENCODE_CHARS, c);
        if (!p) throw std::runtime_error("Invalid Base85 character");
        return static_cast<uint32_t>(p - ENCODE_CHARS);
    };

    for (size_t i = 0; i < n; i += 5) {
        uint32_t val = 0;
        int count = 0;
        
        for (int j = 0; j < 5; ++j) {
            val *= 85;
            if (i + j < n) {
                val += char_to_val(b85str[i + j]);
                count++;
            } else {
                val += 84; // Padding
            }
        }
        
        if (count < 2) break; // Недостаточно данных для даже одного байта

        std::array<uint8_t, 4> block;
        for (int j = 3; j >= 0; --j) {
            block[j] = val & 0xFF;
            val >>= 8;
        }
        
        for (int j = 0; j < count - 1; ++j) {
            result.push_back(block[j]);
        }
    }
    return result;
}

}
