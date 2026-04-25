#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include "base85ed.h"

std::vector<uint8_t> read_stdin() {
    std::vector<uint8_t> out;
    uint8_t buf[4096];
    while (std::cin.read(reinterpret_cast<char*>(buf), sizeof(buf)) || std::cin.gcount() > 0) {
        out.insert(out.end(), buf, buf + std::cin.gcount());
    }
    return out;
}

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [-e | -d]\n";
        return 1;
    }

    std::string arg = argv[1];
    std::function<std::vector<uint8_t>(const std::vector<uint8_t>&)> func;

    if (arg == "-e" || arg == "--encode") func = base85::encode;
    else if (arg == "-d" || arg == "--decode") func = base85::decode;
    else {
        std::cerr << "Unknown argument: " << arg << "\n";
        return 1;
    }

    auto input = read_stdin();
    try {
        auto output = func(input);
        std::cout.write(reinterpret_cast<const char*>(output.data()), output.size());
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
