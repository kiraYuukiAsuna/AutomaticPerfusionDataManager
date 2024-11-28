#pragma once

#include <fstream>
#include <iostream>
#include "json.hpp"

struct Config {
    std::filesystem::path DataStoragePath;

    friend void to_json(nlohmann::json&jsonObject,
                        const Config&dataObject) {
        jsonObject["DataStoragePath"] = dataObject.DataStoragePath;
    }

    friend void from_json(const nlohmann::json&jsonObject,
                          Config&dataObject) {
        Config defaultDataObject;
        dataObject.DataStoragePath =
                jsonObject.value("DataStoragePath", defaultDataObject.DataStoragePath);
    }
};

class ConfigManager {
private:
    ConfigManager() = default;

    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
    ConfigManager(ConfigManager&&) = delete;
    ConfigManager& operator=(ConfigManager&&) = delete;

    Config m_Config;

public:
    ~ConfigManager() = default;

    static ConfigManager& GetInstance() {
        static ConfigManager instance;
        return instance;
    }

    std::filesystem::path GetCurrentConfigPath() {
        return std::filesystem::current_path() / "AppConfig.json";
    }

    Config& GetConfig() {
        return m_Config;
    }

    void SetConfig(const Config& config) {
        m_Config = config;
    }

    void ReadConfigFromFile() {
        if(!std::filesystem::exists(GetCurrentConfigPath())) {
            std::ofstream file(GetCurrentConfigPath());
            if (!file.is_open()) {
                throw std::runtime_error(std::format("Failed to create file: {}", GetCurrentConfigPath().string()));
            }
            Config defaultConfig;
            file << nlohmann::json(defaultConfig);
            file.close();
        }
        std::ifstream file(GetCurrentConfigPath());
        if (!file.is_open()) {
            throw std::runtime_error(std::format("Failed to open file: {}", GetCurrentConfigPath().string()));
        }

        nlohmann::json json;
        file >> json;

        m_Config = json;

        file.close();
    }

    void WriteConfigToFile() {
        std::ofstream file(GetCurrentConfigPath());
        if (!file.is_open()) {
            throw std::runtime_error(std::format("Failed to open file: {}", GetCurrentConfigPath().string()));
        }

        file << nlohmann::json(m_Config);

        file.close();
    }
};
