#pragma once

#include <rapidcsv.h>

#include "json.hpp"
#include <string>
#include <vector>

struct StateTimeDifferenceInfo {
    int CellId = 0;
    float CellDepth = 0;
    int IsBadCell = false;
    double CellArea = 0;
    double CellRadius = 0;
    float InjectionDepth = 0;
    std::vector<std::pair<std::string, time_t>> StatesTime;

    friend void to_json(nlohmann::json&jsonObject, const StateTimeDifferenceInfo&dataObject) {
        jsonObject["CellId"] = dataObject.CellId;
        jsonObject["CellDepth"] = dataObject.CellDepth;
        jsonObject["IsBadCell"] = dataObject.IsBadCell;
        jsonObject["CellArea"] = dataObject.CellArea;
        jsonObject["CellRadius"] = dataObject.CellRadius;
        jsonObject["InjectionDepth"] = dataObject.InjectionDepth;
        jsonObject["StatesTime"] = dataObject.StatesTime;
    }

    friend void from_json(const nlohmann::json&jsonObject, StateTimeDifferenceInfo&dataObject) {
        StateTimeDifferenceInfo defaultDataObject;
        dataObject.CellId = jsonObject.value("CellId", defaultDataObject.CellId);
        dataObject.CellDepth = jsonObject.value("CellDepth", defaultDataObject.CellDepth);
        dataObject.IsBadCell = jsonObject.value("IsBadCell", defaultDataObject.IsBadCell);
        dataObject.CellArea = jsonObject.value("CellArea", defaultDataObject.CellArea);
        dataObject.CellRadius = jsonObject.value("CellRadius", defaultDataObject.CellRadius);
        dataObject.InjectionDepth = jsonObject.value("InjectionDepth", defaultDataObject.InjectionDepth);
        dataObject.StatesTime = jsonObject.value("StatesTime", defaultDataObject.StatesTime);
    }
};

using StateTimeDifferenceInfoList = std::vector<StateTimeDifferenceInfo>;

StateTimeDifferenceInfoList ReadStateTimeDifferenceInfoListFromFile(const std::string&filePath) {
    rapidcsv::Document doc(filePath, rapidcsv::LabelParams(0, -1));

    StateTimeDifferenceInfoList stateTimeDifferenceInfoList;
    for (int row = 0; row < doc.GetRowCount(); row++) {
        StateTimeDifferenceInfo stateTimeDifferenceInfo;
        stateTimeDifferenceInfo.CellId = doc.GetCell<int>(0, row);
        stateTimeDifferenceInfo.CellDepth = doc.GetCell<float>(1, row);
        stateTimeDifferenceInfo.IsBadCell = doc.GetCell<int>(2, row);
        stateTimeDifferenceInfo.CellArea = doc.GetCell<double>(3, row);
        stateTimeDifferenceInfo.CellRadius = doc.GetCell<double>(4, row);
        stateTimeDifferenceInfo.InjectionDepth = doc.GetCell<float>(5, row);

        for (int col = 6; col < doc.GetColumnCount(); col++) {
            stateTimeDifferenceInfo.StatesTime.emplace_back(doc.GetColumnName(col), doc.GetCell<time_t>(6, col));
        }

        stateTimeDifferenceInfoList.emplace_back(stateTimeDifferenceInfo);
    }
    return stateTimeDifferenceInfoList;
}
