#pragma once
#include <filesystem>

#include "Config/Config.hpp"

class CellInformationRepository {
public:
    static std::filesystem::path GetCellInformationRepositoryPath() {
        ConfigManager::GetInstance().ReadConfigFromFile();
        auto config = ConfigManager::GetInstance().GetConfig();
        return config.DataStoragePath / "CellInformationRepository";
    }
};
