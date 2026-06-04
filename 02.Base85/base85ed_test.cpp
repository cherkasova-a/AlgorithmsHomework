#include <gtest/gtest.h>
#include <vector>
#include <cstdint>
#include <string>
#include "base85ed.h"

const std::vector<std::pair<const char *, const char * >> short_cases =
{
    { "",     ""     },
    { "F#",   "1"    },
    { "F){",  "12"   },
    { "F)}j", "123"  },
    { "F)}kW","1234" }
};

static std::vector<uint8_t> cstr2v(const char *s)
{
    return std::vector<uint8_t>(s, s + std::string(s).size());
}

TEST(Base85ShortsEncode, TrivialShortEncodes)
{
    for (const auto &p : short_cases)
    {
        EXPECT_EQ(base85::encode(cstr2v(p.second)), cstr2v(p.first));
    }
}

TEST(Base85ShortsDecode, TrivialShortDecodes)
{
    for (const auto &p : short_cases)
    {
        EXPECT_EQ(base85::decode(cstr2v(p.first)), cstr2v(p.second));
    }
}

TEST(Base85EvilTests, BinaryZerosAndStructure)
{
    std::vector<uint8_t> evil_data = {0, 0, 0, 0, 255, 255, 127, 0, 1, 2, 3};
    auto encoded = base85::encode(evil_data);
    auto decoded = base85::decode(encoded);
    EXPECT_EQ(evil_data, decoded);
}

TEST(Base85EvilTests, InvalidCharacters)
{
    std::vector<uint8_t> invalid_b85 = cstr2v("F)}k\n");
    EXPECT_THROW(base85::decode(invalid_b85), std::runtime_error);
}
