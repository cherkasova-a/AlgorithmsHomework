#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>

std::string encode_base85(const std::vector<uint8_t>& data) {
  std::string result;
  size_t i = 0;
  for (; i + 4 <= data.size(); i += 4) {
    uint32_t num = (data[i] << 24) | (data[i + 1] << 16) | (data[i + 2] << 8) | data[i + 3];
    if (num == 0) {
      result += 'z';
    } else {
      std::string block;
      for (int j = 0; j < 5; ++j) {
        block = static_cast<char>('!' + (num % 85)) + block;
        num /= 85;
      }
      result += block;
    }
  }
  if (i < data.size()) {
    size_t padding = data.size() - i;
    uint32_t num = 0;
    for (size_t j = 0; j < 4; ++j) {
      num <<= 8;
      if (j < padding) {
        num |= data[i + j];
      }
    }
    std::string block;
    for (int j = 0; j < 5; ++j) {
      block = static_cast<char>('!' + (num % 85)) + block;
      num /= 85;
    }
    result += block.substr(0, padding + 1);
  }
  return result;
}

std::vector<uint8_t> decode_base85(const std::string& str) {
  std::vector<uint8_t> result;
  size_t i = 0;
  while (i < str.size()) {
    while (i < str.size() && (str[i] == ' ' || str[i] == '\n' || str[i] == '\r' || str[i] == '\t')) {
      i++;
    }
    if (i >= str.size()) break;
    if (str[i] == 'z') {
      for (int j = 0; j < 4; ++j) result.push_back(0);
      i++;
      continue;
    }
    uint64_t num = 0;
    size_t count = 0;
    size_t start_i = i;
    while (i < str.size() && count < 5) {
      char c = str[i];
      if (c == ' ' || c == '\n' || c == '\r' || c == '\t') {
        i++;
        continue;
      }
      if (c < '!' || c > 'u') {
        i++;
        continue;
      }
      num = num * 85 + (c - '!');
      count++;
      i++;
    }
    if (count == 5) {
      result.push_back((num >> 24) & 0xFF);
      result.push_back((num >> 16) & 0xFF);
      result.push_back((num >> 8) & 0xFF);
      result.push_back(num & 0xFF);
    } else if (count > 1) {
      size_t padding = 5 - count;
      for (size_t j = 0; j < padding; ++j) {
        num = num * 85 + 84;
      }
      for (size_t j = 0; j < count - 1; ++j) {
        result.push_back((num >> (24 - j * 8)) & 0xFF);
      }
    }
  }
  return result;
}

int main(int argc, char* argv[]) {
  if (argc < 4) return 1;
  std::string mode = argv[1];
  std::string infile = argv[2];
  std::string outfile = argv[3];
  std::ifstream in(infile, std::ios::binary);
  std::ofstream out(outfile, std::ios::binary);
  if (!in.is_open() || !out.is_open()) return 1;
  std::vector<uint8_t> data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
  if (mode == "-e") {
    out << encode_base85(data);
  } else if (mode == "-d") {
    std::string str(data.begin(), data.end());
    std::vector<uint8_t> decoded = decode_base85(str);
    out.write(reinterpret_cast<const char*>(decoded.data()), decoded.size());
  }
  return 0;
}
