#pragma once

#include <rapidcsv.h>

#include <Database/SqliteSchema.hpp>
#include <Util/Util.hpp>
#include <filesystem>

#include "Database/CellInformationRepository.hpp"
#include "Log/Log.h"

class DataImporter {
public:
    std::vector<std::filesystem::path> m_TissueFolders;

    using CallBackType = std::function<void(bool, const std::string&)>;

    int importRecordFromFolder(const std::filesystem::path&folderPath, CallBackType callback) {
        int dbCellTissueInsertCount = 0;
        try {
            for (const auto&tissueFolderEntry:
                 std::filesystem::directory_iterator(folderPath)) {
                if (tissueFolderEntry.is_directory()) {
                    m_TissueFolders.emplace_back(tissueFolderEntry.path());
                }
            }

            for (const auto&tissueFolder: m_TissueFolders) {
                SeeleInfo("{}", tissueFolder.string());
                int inserted = importCellTissueFile(tissueFolder);
                dbCellTissueInsertCount += inserted;
            }

            callback(true, "导入数据成功！" + std::to_string(dbCellTissueInsertCount) + "条新数据已导入！");
        }
        catch (std::exception&e) {
            callback(false, std::string{"导入数据失败！"} + e.what());
        }

        return dbCellTissueInsertCount;
    }

    int importCellTissueFile(const std::filesystem::path&tissueFolder) {
        int dbCellTissueInsertCount = 0;

        std::filesystem::path cellMapFilePath;
        std::filesystem::path cellTissueFilePath;
        int date = 0;
        int time = 0;
        std::vector<std::pair<int, std::filesystem::path>>
                stateMachineDifferencesFiles;
        std::map<int, std::filesystem::path> cellMap;

        for (const auto&recordFolderEntry:
             std::filesystem::directory_iterator(tissueFolder)) {
            if (recordFolderEntry.is_directory()) {
                auto fileName = recordFolderEntry.path().stem().string();
                auto splitResult = StringSplit(fileName, '_');

                if (splitResult.size() <= 2) {
                    splitResult = StringSplit(fileName, '-');
                }

                if (splitResult.size() == 3) {
                    int cellId;
                    try {
                        cellId = std::stoi(splitResult[splitResult.size() - 1]);

                        auto iter = cellMap.find(cellId);
                        if (iter != cellMap.end()) {
                            int curDate = std::stoi(splitResult[0]);
                            int curTime = std::stoi(splitResult[1]);

                            auto lastFileName = iter->second.stem().string();
                            auto sp = StringSplit(lastFileName, '_');

                            if (sp.size() <= 2) {
                                sp = StringSplit(fileName, '-');
                            }

                            int lastDate = std::stoi(sp[0]);
                            int lastTTime = std::stoi(sp[1]);

                            if (curDate > lastDate ||
                                (curDate == lastDate && curTime > lastTTime)) {
                                cellMap[cellId] = recordFolderEntry.path();
                            }
                        }
                        else {
                            cellMap.emplace(cellId, recordFolderEntry.path());
                        }
                    }
                    catch (std::exception&e) {
                        SeeleError("{}", e.what());
                    }
                }
            }
            else if (recordFolderEntry.is_regular_file()) {
                if (recordFolderEntry.path().extension() == ".csv") {
                    auto fileName = recordFolderEntry.path().stem().string();
                    if (fileName.find(tissueFolder.filename().string()) !=
                        std::string::npos) {
                        auto splitResult = StringSplit(fileName, '_');

                        if (splitResult.size() <= 2) {
                            splitResult = StringSplit(fileName, '-');
                        }

                        int curDate = 0;
                        int curTime = 0;
                        try {
                            curDate =
                                    std::stoi(splitResult[splitResult.size() - 2]);
                            curTime =
                                    std::stoi(splitResult[splitResult.size() - 1]);
                        }
                        catch (std::exception&e) {
                            SeeleError("{}", e.what());
                        }
                        if (curDate > date ||
                            (curDate == date && curTime > time)) {
                            date = curDate;
                            time = curTime;
                            cellTissueFilePath = recordFolderEntry.path();
                        }
                    }
                    else if (fileName.find("state_time_differences") !=
                             std::string::npos) {
                        auto splitResult = StringSplit(fileName, '_');

                        if (splitResult.size() <= 2) {
                            splitResult = StringSplit(fileName, '-');
                        }

                        int needleId = -1;
                        try {
                            needleId =
                                    std::stoi(splitResult[splitResult.size() - 1]);
                        }
                        catch (std::exception&e) {
                            SeeleError("{}", e.what());
                        }
                        if (needleId != -1) {
                            stateMachineDifferencesFiles.emplace_back(
                                needleId, recordFolderEntry.path());
                        }
                    }
                }
                else if (recordFolderEntry.path().extension() == ".json") {
                    auto fileName = recordFolderEntry.path().stem().string();
                    if (fileName.find("CellMap") != std::string::npos) {
                        cellMapFilePath = recordFolderEntry.path();
                    }
                }
            }
        }

        auto db = SqliteSchema::getDBStorage();

        auto cellTissueInfos = ReadCellTissueListFromFile(cellTissueFilePath);

        std::map<int, std::pair<StateTimeDifferenceInfo,std::string>> stateMachineDifferencesInfos;
        for (auto&[needleName, stateFile]: stateMachineDifferencesFiles) {
            auto state = ReadStateTimeDifferenceInfoListFromFile(stateFile);
            for (auto&stateInfo: state) {
                stateInfo.TissueName = tissueFolder.filename().string();
                auto sp = StringSplit(stateFile.stem().string(), '_');
                if(!sp.empty()) {
                    auto needleNameStr = sp[sp.size() - 1];
                    stateMachineDifferencesInfos.emplace(stateInfo.CellId, std::pair(stateInfo,needleNameStr));

                }
            }
        }

        for (auto&cellTissueInfo: cellTissueInfos) {
            try {
                int id = db.insert(cellTissueInfo);
                if (id != -1) {
                    dbCellTissueInsertCount++;

                    auto sp = StringSplit(cellTissueInfo.TissueCellID, '_');
                    auto cellId = sp[sp.size() - 1];
                    if (!cellId.empty() && cellId.at(0) == 'C') {
                        cellId = cellId.substr(1, cellId.size() - 1);
                        auto cellInfoFolderIter = cellMap.find(std::stoi(cellId));

                        if (cellInfoFolderIter != cellMap.end()) {
                            std::filesystem::copy(cellInfoFolderIter->second,
                                                  CellInformationRepository::GetCellInformationRepositoryPath() /
                                                  cellTissueInfo.TissueCellID);
                        }

                        auto stateInfoIter = stateMachineDifferencesInfos.find(std::stoi(cellId));
                        if (stateInfoIter != stateMachineDifferencesInfos.end()) {
                            auto stateInfo = stateInfoIter->second.first;
                            stateInfo.TissueName = tissueFolder.stem().string();
                            stateInfo.NeedleName = stateInfoIter->second.second;
                            db.insert(stateInfo);
                        }
                    }
                }
            }
            catch (std::exception&e) {
                // SeeleError("{}", e.what());
            }
        }

        // for (auto& [needleName, stateFile] : stateMachineDifferencesFiles) {
        // 	auto state = ReadStateTimeDifferenceInfoListFromFile(stateFile);
        // 	for (auto& stateInfo : state) {
        // 		stateInfo.TissueName = tissueFolder.filename().string();
        // 		db.insert(stateInfo);
        // 	}
        // }

        return dbCellTissueInsertCount;
    }
};
