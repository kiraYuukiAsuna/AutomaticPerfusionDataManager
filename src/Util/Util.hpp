#pragma once

#include <ranges>
#include <string>
#include <vector>

inline std::vector<std::string> StringSplit(const std::string&s,
                                            char delim = ' ') {
    std::vector<std::string> tokens;
    for (auto token: s | std::views::split(delim)) {
        tokens.emplace_back(token.begin(), token.end());
    }
    return tokens;
}

inline std::time_t ParseDateTime(const std::string&date, const std::string&time) {
    std::string dateTimeStr = date + " " + time;
    std::tm tm = {};
    std::istringstream ss(dateTimeStr);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    return std::mktime(&tm);
}

inline std::pair<std::string, std::string> FormatDateTime(std::time_t time) {
    std::tm* tm = std::localtime(&time);
    std::ostringstream ossDate, ossTime;
    ossDate << std::put_time(tm, "%Y-%m-%d");
    ossTime << std::put_time(tm, "%H:%M:%S");
    return {ossDate.str(), ossTime.str()};
}
