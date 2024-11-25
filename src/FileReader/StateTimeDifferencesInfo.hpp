#pragma once

#include <rapidcsv.h>

#include <string>
#include <vector>

#include "json.hpp"

struct StateTimeDifferenceInfo {
	int id = -1;
	std::string TissueName;
	std::string NeedleName;

	int CellId = 0;
	float CellDepth = 0;
	int IsBadCell = false;
	double CellArea = 0;
	double CellRadius = 0;
	float InjectionDepth = 0;

	time_t START;
	time_t MOVE_TO_CELL;
	time_t AUTO_CALIBRATE;
	time_t RETREAT_NEEDLE;
	time_t FIND_CELL;
	time_t LOWER_NEEDLE;
	time_t AXIAL_INJECTCELL;
	time_t START_PERFUSE;
	time_t WAIT;
	time_t WITHDRAW_NEEDLE;
	time_t LIFT_NEEDLE;
	time_t END;

	friend void to_json(nlohmann::json& jsonObject,
						const StateTimeDifferenceInfo& dataObject) {
		jsonObject["NeedleName"] = dataObject.NeedleName;
		jsonObject["CellId"] = dataObject.CellId;
		jsonObject["CellDepth"] = dataObject.CellDepth;
		jsonObject["IsBadCell"] = dataObject.IsBadCell;
		jsonObject["CellArea"] = dataObject.CellArea;
		jsonObject["CellRadius"] = dataObject.CellRadius;
		jsonObject["InjectionDepth"] = dataObject.InjectionDepth;
		jsonObject["START"] = dataObject.START;
		jsonObject["MOVE_TO_CELL"] = dataObject.MOVE_TO_CELL;
		jsonObject["AUTO_CALIBRATE"] = dataObject.AUTO_CALIBRATE;
		jsonObject["RETREAT_NEEDLE"] = dataObject.RETREAT_NEEDLE;
		jsonObject["FIND_CELL"] = dataObject.FIND_CELL;
		jsonObject["LOWER_NEEDLE"] = dataObject.LOWER_NEEDLE;
		jsonObject["AXIAL_INJECTCELL"] = dataObject.AXIAL_INJECTCELL;
		jsonObject["START_PERFUSE"] = dataObject.START_PERFUSE;
		jsonObject["WAIT"] = dataObject.WAIT;
		jsonObject["WITHDRAW_NEEDLE"] = dataObject.WITHDRAW_NEEDLE;
		jsonObject["LIFT_NEEDLE"] = dataObject.LIFT_NEEDLE;
		jsonObject["END"] = dataObject.END;
	}

	friend void from_json(const nlohmann::json& jsonObject,
						  StateTimeDifferenceInfo& dataObject) {
		StateTimeDifferenceInfo defaultDataObject;
		dataObject.NeedleName =
	jsonObject.value("NeedleName", defaultDataObject.NeedleName);
		dataObject.CellId =
			jsonObject.value("CellId", defaultDataObject.CellId);
		dataObject.CellDepth =
			jsonObject.value("CellDepth", defaultDataObject.CellDepth);
		dataObject.IsBadCell =
			jsonObject.value("IsBadCell", defaultDataObject.IsBadCell);
		dataObject.CellArea =
			jsonObject.value("CellArea", defaultDataObject.CellArea);
		dataObject.CellRadius =
			jsonObject.value("CellRadius", defaultDataObject.CellRadius);
		dataObject.InjectionDepth = jsonObject.value(
			"InjectionDepth", defaultDataObject.InjectionDepth);
		dataObject.START = jsonObject.value("START", defaultDataObject.START);
		dataObject.MOVE_TO_CELL =
			jsonObject.value("MOVE_TO_CELL", defaultDataObject.MOVE_TO_CELL);
		dataObject.AUTO_CALIBRATE = jsonObject.value(
			"AUTO_CALIBRATE", defaultDataObject.AUTO_CALIBRATE);
		dataObject.RETREAT_NEEDLE = jsonObject.value(
			"RETREAT_NEEDLE", defaultDataObject.RETREAT_NEEDLE);
		dataObject.FIND_CELL =
			jsonObject.value("FIND_CELL", defaultDataObject.FIND_CELL);
		dataObject.LOWER_NEEDLE =
			jsonObject.value("LOWER_NEEDLE", defaultDataObject.LOWER_NEEDLE);
		dataObject.AXIAL_INJECTCELL = jsonObject.value(
			"AXIAL_INJECTCELL", defaultDataObject.AXIAL_INJECTCELL);
		dataObject.START_PERFUSE =
			jsonObject.value("START_PERFUSE", defaultDataObject.START_PERFUSE);
		dataObject.WAIT = jsonObject.value("WAIT", defaultDataObject.WAIT);
		dataObject.WITHDRAW_NEEDLE = jsonObject.value(
			"WITHDRAW_NEEDLE", defaultDataObject.WITHDRAW_NEEDLE);
		dataObject.LIFT_NEEDLE =
			jsonObject.value("LIFT_NEEDLE", defaultDataObject.LIFT_NEEDLE);
		dataObject.END = jsonObject.value("END", defaultDataObject.END);
	}
};

using StateTimeDifferenceInfoList = std::vector<StateTimeDifferenceInfo>;

inline StateTimeDifferenceInfoList ReadStateTimeDifferenceInfoListFromFile(
	const std::filesystem::path& filePath) {
	rapidcsv::Document doc(filePath.string(), rapidcsv::LabelParams(0, -1));

	StateTimeDifferenceInfoList stateTimeDifferenceInfoList;
	for (int row = 0; row < doc.GetRowCount(); row++) {
		StateTimeDifferenceInfo stateTimeDifferenceInfo;
		stateTimeDifferenceInfo.CellId = doc.GetCell<int>(0, row);
		stateTimeDifferenceInfo.CellDepth = doc.GetCell<float>(1, row);
		stateTimeDifferenceInfo.IsBadCell = doc.GetCell<int>(2, row);
		stateTimeDifferenceInfo.CellArea = doc.GetCell<double>(3, row);
		stateTimeDifferenceInfo.CellRadius = doc.GetCell<double>(4, row);
		stateTimeDifferenceInfo.InjectionDepth = doc.GetCell<float>(5, row);
		stateTimeDifferenceInfo.START = doc.GetCell<time_t>(6, row);
		stateTimeDifferenceInfo.MOVE_TO_CELL = doc.GetCell<time_t>(7, row);
		stateTimeDifferenceInfo.AUTO_CALIBRATE = doc.GetCell<time_t>(8, row);
		stateTimeDifferenceInfo.RETREAT_NEEDLE = doc.GetCell<time_t>(9, row);
		stateTimeDifferenceInfo.FIND_CELL = doc.GetCell<time_t>(10, row);
		stateTimeDifferenceInfo.LOWER_NEEDLE = doc.GetCell<time_t>(11, row);
		stateTimeDifferenceInfo.AXIAL_INJECTCELL = doc.GetCell<time_t>(12, row);
		stateTimeDifferenceInfo.START_PERFUSE = doc.GetCell<time_t>(13, row);
		stateTimeDifferenceInfo.WAIT = doc.GetCell<time_t>(14, row);
		stateTimeDifferenceInfo.WITHDRAW_NEEDLE = doc.GetCell<time_t>(15, row);
		stateTimeDifferenceInfo.LIFT_NEEDLE = doc.GetCell<time_t>(16, row);
		stateTimeDifferenceInfo.END = doc.GetCell<time_t>(17, row);

		if(doc.GetColumnCount() >= 19) {
			stateTimeDifferenceInfo.NeedleName = doc.GetCell<std::string>(18, row);
		}

		stateTimeDifferenceInfoList.emplace_back(stateTimeDifferenceInfo);
	}
	return stateTimeDifferenceInfoList;
}
