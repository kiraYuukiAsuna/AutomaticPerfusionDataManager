#pragma once

#include "json.hpp"
#include <vector>

struct CellMapInfo {
    int id = -1;
    std::string TissueName;

    int              MapIndex = 0;
    float            MapBeginCordX = 0;
    float            MapBeginCordY = 0;
    float            MapEndCordX = 0;
    float            MapEndCordY = 0;
    int              PathIndex = 0;
    float            SurfaceZ = 0;
    std::vector<int> needleIndexs;

    friend void to_json(nlohmann::json& jsonObject, const CellMapInfo& dataObject) {
        jsonObject["MapIndex"] = dataObject.MapIndex;
        jsonObject["MapBeginCordX"] = dataObject.MapBeginCordX;
        jsonObject["MapBeginCordY"] = dataObject.MapBeginCordY;
        jsonObject["MapEndCordX"] = dataObject.MapEndCordX;
        jsonObject["MapEndCordY"] = dataObject.MapEndCordY;
        jsonObject["PathIndex"] = dataObject.PathIndex;
        jsonObject["SurfaceZ"] = dataObject.SurfaceZ;
        jsonObject["needleIndexs"] = dataObject.needleIndexs;
    }

    friend void from_json(const nlohmann::json& jsonObject, CellMapInfo& dataObject) {
        CellMapInfo defaultDataObject;
        dataObject.MapIndex = jsonObject.value("MapIndex", defaultDataObject.MapIndex);
        dataObject.MapBeginCordX = jsonObject.value("MapBeginCordX", defaultDataObject.MapBeginCordX);
        dataObject.MapBeginCordY = jsonObject.value("MapBeginCordY", defaultDataObject.MapBeginCordY);
        dataObject.MapEndCordX = jsonObject.value("MapEndCordX", defaultDataObject.MapEndCordX);
        dataObject.MapEndCordY = jsonObject.value("MapEndCordY", defaultDataObject.MapEndCordY);
        dataObject.PathIndex = jsonObject.value("PathIndex", defaultDataObject.PathIndex);
        dataObject.SurfaceZ = jsonObject.value("SurfaceZ", defaultDataObject.SurfaceZ);
        dataObject.needleIndexs = jsonObject.value("needleIndexs", defaultDataObject.needleIndexs);
    }
};

using CellMapInfoList = std::vector<CellMapInfo>;
