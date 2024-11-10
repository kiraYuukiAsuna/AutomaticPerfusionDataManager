#pragma once

#include <rapidcsv.h>

#include <Database/SqliteSchema.hpp>
#include <Util/Util.hpp>
#include <filesystem>

class DataImporter {
public:
	std::vector<std::filesystem::path> m_TissueFolders;

	void importRecordFromFolder(const std::filesystem::path& folderPath) {
		for (const auto& tissueFolderEntry :
			 std::filesystem::directory_iterator(folderPath)) {
			if (tissueFolderEntry.is_directory()) {
				m_TissueFolders.emplace_back(tissueFolderEntry.path());
			}
		}

		for (const auto& tissueFolder : m_TissueFolders) {
			importCellTissueFile(tissueFolder);
		}
	}

	void importCellTissueFile(const std::filesystem::path& tissueFolder) {
		std::filesystem::path cellMapFilePath;
		std::filesystem::path cellTissueFilePath;
		int date = 0;
		int time = 0;
		std::vector<std::pair<int, std::filesystem::path>>
			stateMachineDifferencesFiles;
		std::map<int, std::filesystem::path> cellMap;

		for (const auto& recordFolderEntry :
			 std::filesystem::directory_iterator(tissueFolder)) {
			if (recordFolderEntry.is_directory()) {
				auto fileName = recordFolderEntry.path().stem().string();
				auto splitResult = StringSplit(fileName, '_');

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
							int lastDate = std::stoi(sp[0]);
							int lastTTime = std::stoi(sp[0]);

							if (curDate > lastDate ||
								(curDate == lastDate && curTime > lastTTime)) {
								cellMap[cellId] = recordFolderEntry.path();
							}
						} else {
							cellMap.emplace(cellId, recordFolderEntry.path());
						}
					} catch (std::exception& e) {
					}
				}
			} else if (recordFolderEntry.is_regular_file()) {
				if (recordFolderEntry.path().extension() == ".csv") {
					auto fileName = recordFolderEntry.path().stem().string();
					if (fileName.find(tissueFolder.filename().string()) !=
						std::string::npos) {
						auto splitResult = StringSplit(fileName, '_');
						int curDate = 0;
						int curTime = 0;
						try {
							curDate =
								std::stoi(splitResult[splitResult.size() - 2]);
							curTime =
								std::stoi(splitResult[splitResult.size() - 1]);
						} catch (std::exception& e) {
						}
						if (curDate > date ||
							(curDate == date && curTime > time)) {
							date = curDate;
							time = curTime;
							cellTissueFilePath = recordFolderEntry.path();
						}
					} else if (fileName.find("state_time_differences") !=
							   std::string::npos) {
						auto splitResult = StringSplit(fileName, '_');
						int needleId = -1;
						try {
							needleId =
								std::stoi(splitResult[splitResult.size() - 1]);
						} catch (std::exception& e) {
						}
						if (needleId != -1) {
							stateMachineDifferencesFiles.emplace_back(
								needleId, recordFolderEntry.path());
						}
					}
				} else if (recordFolderEntry.path().extension() == ".json") {
					auto fileName = recordFolderEntry.path().stem().string();
					if (fileName.find("CellMap") != std::string::npos) {
						cellMapFilePath = recordFolderEntry.path();
					}
				}
			}
		}

		auto db = SqliteSchema::getDBStorage();
		db.sync_schema();

		auto cellTissueInfos = ReadCellTissueListFromFile(cellTissueFilePath);
		for (auto& cellTissueInfo : cellTissueInfos) {
			try {
				db.insert(cellTissueInfo);
			} catch (std::exception& e) {
				std::cout << e.what() << std::endl;
			}
		}

		for (auto& [needleName, stateFile] : stateMachineDifferencesFiles) {
			auto state = ReadStateTimeDifferenceInfoListFromFile(stateFile);
			for (auto& stateInfo : state) {
				stateInfo.TissueName = tissueFolder.filename().string();
				db.insert(stateInfo);
			}
		}
	}
};
