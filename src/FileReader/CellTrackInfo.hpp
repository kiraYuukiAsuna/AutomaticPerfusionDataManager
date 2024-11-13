#pragma once

#include <string>
#include <vector>

#include "json.hpp"

struct CellTrackInfo {
	int id = -1;
	std::string TissueName;

	std::string CellID;
	float CordX = 0;
	float CordY = 0;
	float CordZ = 0;
	time_t TimeStamp = 0;
	float CellArea = 0;

	friend void to_json(nlohmann::json& jsonObject,
						const CellTrackInfo& dataObject) {
		jsonObject["CellID"] = dataObject.CellID;
		jsonObject["CordX"] = dataObject.CordX;
		jsonObject["CordY"] = dataObject.CordY;
		jsonObject["CordZ"] = dataObject.CordZ;
		jsonObject["TimeStamp"] = dataObject.TimeStamp;
		jsonObject["CellArea"] = dataObject.CellArea;
	}

	friend void from_json(const nlohmann::json& jsonObject,
						  CellTrackInfo& dataObject) {
		CellTrackInfo defaultDataObject;
		dataObject.CellID =
			jsonObject.value("CellID", defaultDataObject.CellID);
		dataObject.CordX = jsonObject.value("CordX", defaultDataObject.CordX);
		dataObject.CordY = jsonObject.value("CordY", defaultDataObject.CordY);
		dataObject.CordZ = jsonObject.value("CordZ", defaultDataObject.CordZ);
		dataObject.TimeStamp =
			jsonObject.value("TimeStamp", defaultDataObject.TimeStamp);
		dataObject.CellArea =
			jsonObject.value("CellArea", defaultDataObject.CellArea);
	}
};

using CellTrackInfoList = std::vector<CellTrackInfo>;
