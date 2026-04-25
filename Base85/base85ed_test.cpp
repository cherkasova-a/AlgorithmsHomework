#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "base85ed.h"

static std::vector<uint8_t> s2v(const std::string& s) {
    return std::vector<uint8_t>(s.begin(), s.end());
}

const std::vector<std::pair<std::string, std::string>> short_cases = {
    { "",       ""     },
    { "F#",     "1"    },
    { "F){",    "12"   },
    { "F)}j",   "123"  },
    { "F)}kW",  "1234" }
};

TEST(Base85Test, EncodeShortCases) {
    for (const auto &p : short_cases) {
        EXPECT_EQ(base85::encode(s2v(p.second)), s2v(p.first));
    }
}

TEST(Base85Test, DecodeShortCases) {
    for (const auto &p : short_cases) {
        EXPECT_EQ(base85::decode(s2v(p.first)), s2v(p.second));
    }
}

TEST(Base85Test, RoundTrip) {
    std::string original = "Artificial Intelligence @ SPbU";
    auto encoded = base85::encode(s2v(original));
    auto decoded = base85::decode(encoded);
    EXPECT_EQ(s2v(original), decoded);
}

TEST(Base85Test, InvalidInput) {
    EXPECT_THROW(base85::decode(s2v("invalid char")), std::runtime_error);
}
