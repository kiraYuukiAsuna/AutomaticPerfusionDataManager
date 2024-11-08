#pragma once

#include <vector>
#include <string>
#include <ranges>

inline std::vector<std::string> StringSplit(const std::string&s, char delim = ' ') {
    std::vector<std::string> tokens;
    for (auto token: s | std::views::split(delim)) {
        tokens.emplace_back(token.begin(), token.end());
    }
    return tokens;
}
