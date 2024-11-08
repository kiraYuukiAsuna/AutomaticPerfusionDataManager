#pragma once

#include "json.hpp"
#include <string>

struct NeedleTrackInfo {
    int id = -1;
    std::string TissueName;
    int CellId = -1;

    std::string NeedleName;
    float       CordX = 0;
    float       CordY = 0;
    float       CordZ = 0;
    time_t      TimeStamp = 0;

    friend void to_json(nlohmann::json& jsonObject, const NeedleTrackInfo& dataObject) {
        jsonObject["NeedleName"] = dataObject.NeedleName;
        jsonObject["CordX"] = dataObject.CordX;
        jsonObject["CordY"] = dataObject.CordY;
        jsonObject["CordZ"] = dataObject.CordZ;
        jsonObject["TimeStamp"] = dataObject.TimeStamp;
    }

    friend void from_json(const nlohmann::json& jsonObject, NeedleTrackInfo& dataObject) {
        NeedleTrackInfo defaultDataObject;
        dataObject.NeedleName = jsonObject.value("NeedleName", defaultDataObject.NeedleName);
        dataObject.CordX = jsonObject.value("CordX", defaultDataObject.CordX);
        dataObject.CordY = jsonObject.value("CordY", defaultDataObject.CordY);
        dataObject.CordZ = jsonObject.value("CordZ", defaultDataObject.CordZ);
        dataObject.TimeStamp = jsonObject.value("TimeStamp", defaultDataObject.TimeStamp);
    }
};

using NeedleTrackInfoList = std::vector<NeedleTrackInfo>;
